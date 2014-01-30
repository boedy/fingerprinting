#include "constantqtransform.h"

ConstantQTransform::ConstantQTransform() : fftwPlanFile(0),
beatSynchData(0),
fftOut(0),
powerOut(0),
harmonicityOut(0),
cqtOut(0),
hammingWindow(0),
in(0),
loEdge(0.0),
hiEdge(0.0),
out(0),
p(0),
useWisdom(0),
verbose(0),
power(0),
harmonicity(0),
constantQ(0),
chromagram(0),
cepstrum(0),
beats(0),
CQT(0),
DCT(0),
fftN(0),
winN(0),
hopN(0),
outN(0),
cqtN(0),
dctN(0),
hopTime(0),
usingS(false),
extractChannel(0)
{
    // Usage
    if(argc<3){
        printf("%s [options] inFile outFile\n", argv[0]);
        printf("where options are:\n\n");

        printf("feature options (mutually exclusive):\n");
        printf("-m M : MFCC (Mel-Frequency Cepstral Coefficients) \n");
        printf("-q B : constant-Q spectrum with B bands per octave (0=linear freq.)\n");
        printf("-c C : chromagram folded into C bins\n");
        printf("-P   : overall log power\n");
        printf("-H   : overall log harmonicity\n\n");

        printf("FFT options:\n");
        printf("-l L : low edge of constant-Q transform (63.5444Hz = low C - 50cents)\n");
        printf("-i I : hi edge of constant-Q transform (8kHz)\n");
        printf("-s S : hop size in ms.\n");
        printf("\tA 50%% overlap is used to derive window length and FFT length,\n");
        printf("\twhich will both be the smallest power of 2 number of samples\n");
        printf("\tabove or equal to 2 * the hop size and window length respectively.\n");
        printf("\tGiven in place of -[nwh] (if -s is present they will be ignored.\n");
        printf("-n N : length of FFT (in samples)\n");
        printf("-w W : length of window (in samples) within FFT\n");
        printf("-h H : hop size (in samples)\n\n");

        printf("Extraction options:\n");
        printf("-C c : channel (0=left, 1=right, 2=mix)\n");
        printf("-p file : fftw plan file (generates new if does not exist)\n");
        printf("-b file : beat timing file (secs)\n\n");

        printf("Debug options:\n");
        printf("-v V : verbosity [0 - 10]\n");
        exit(1);
    }
    // Command line options
    processArgs(argc, argv);

    // Open output file
    {
        struct stat buf;
        int code = stat(outFileName, &buf);
        if (code == 0) {
            fprintf(stderr, "Output file exists: %s\n", outFileName);
            exit(1);
        }
    }
    outFile = new ofstream(outFileName, ios::binary);
    if(!outFile){
        printf("Could not open %s for writing\n",outFileName);
        exit(1);
    }
    // Open sound file
    sfinfo.format = 0;
    if (! (inFile = sf_open (inFileName, SFM_READ, &sfinfo))){
        printf ("Not able to open input file %s.\n", inFileName) ;
        puts (sf_strerror (NULL)) ;
        exit(1);
    }
    //if using the semantic hop size, since we now know the sample rate, set dependant params
    if (usingS || (beats && hopTime == 100)){
        if (verbose){
            printf("Sample Rate: %i\n", sfinfo.samplerate);
        }
        hopN = (int)(0.001*hopTime*sfinfo.samplerate);
        winN = nextHigherPow2(hopN*2);
        fftN = winN*2;
        if (verbose) {
            printf("fftN=%d, winN=%d, hopN=%d\n", fftN, winN, hopN);
        }
    }

    if(extractChannel>sfinfo.channels){
        printf("Extract channel exceeds available channels: defaulting to left\n");
        extractChannel=0;
    }


    // Import wisdom
    if(useWisdom) {
        if (!power || !harmonicity) {
            if(!(fftw_import_wisdom_from_file(fftwPlanFile)))
                printf("Warning: failed to import wisdom\n");
        }
    }
    // Output size
    outN = fftN/2+1;

    // Memory allocation
    audioData = new double[sfinfo.channels*winN];
    extractN = outN;
    fftOut = new double[outN]; // Largest possible output is FFT
    extractedData = fftOut;    // Point to FFT output
    makeHammingWindow();
    if (power) {
        extractN = 1;
        powerOut = new double[1];
        extractedData = powerOut;
    } else if (harmonicity) {
        extractN = 1;
        harmonicityOut = new double[1];
        extractedData = harmonicityOut;
    } else if(constantQ) {
        makeLogFreqMap();
        extractN = cqtN; // Modify feature output size here
        cqtOut = new double[cqtN]; // Largest possible output is FFT
        extractedData = cqtOut;  // Point to CQT output
        if(chromagram)
            extractN = constantQ;    // Fold output into one octave
        else if(cepstrum){ // LFCC coefficients
            makeDCT();
            extractN = dctN; // Modify feature output size
        }
    }
    // FFTW memory allocation
    in = (double*) fftw_malloc(sizeof(double)*fftN);
    if (!power || !harmonicity) {
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*outN);
        // FFTW plan
        p = fftw_plan_dft_r2c_1d(fftN, in, out, FFTW_PATIENT);
        if(!useWisdom && fftwPlanFile)
            fftw_export_wisdom_to_file(fftwPlanFile);
    }
    // If beat-synchronus output then make beat-synchronus data buffer
    if(beats){
        beatSynchData = new double[extractN];
        zeroBuf(beatSynchData,extractN);
    }
    // Now perform the extraction
    extract();
}

// Clean up
ConstantQTransform::~fftExtract(){
    // Close input and output files
    if(inFile)
        sf_close(inFile) ;
    if(fftwPlanFile)
        fclose(fftwPlanFile);
    if(p)
        fftw_destroy_plan(p);
    if(beats && beatFile){
        if(beatFile->is_open())
            beatFile->close();
        delete beatFile;
    }
    if(in)
        fftw_free(in);
    if(out)
        fftw_free(out);
    if(audioData)
        delete[] audioData;
    if(fftOut)
        delete[] fftOut;
    if(powerOut)
        delete[] powerOut;
    if(harmonicityOut)
        delete[] harmonicityOut;
    if(cqtOut)
        delete[] cqtOut;
    if(hammingWindow)
        delete[] hammingWindow;
    if(CQT)
        delete[] CQT;
    if(DCT)
        delete[] DCT;
    if(outFile){
        if(outFile->is_open())
            outFile->close();
        delete outFile;
    }
    if(beatSynchData)
        delete[] beatSynchData;
}

void ConstantQTransform::processArgs(int argc, char* argv[]){
    // get file names
    inFileName = argv[argc-2];
    outFileName= argv[argc-1];

    // get command-line options
    for(int k=1; k<argc-2; k++){
        if(strcmp(argv[k], "-P") == 0) {
            power = 1;
        }
        if(strcmp(argv[k], "-H") == 0) {
            harmonicity = 1;
        }
        if(strcmp(argv[k],"-p")==0){
            fftwPlanFileName = argv[k+1];
            fftwPlanFile = fopen(fftwPlanFileName, "r");
            if(fftwPlanFile==0){
                printf("Creating new wisdom\n");
                fftwPlanFile = fopen(fftwPlanFileName, "w");
            }
            else
                useWisdom=1;
            if(fftwPlanFile==0)
                printf("Error creating wisdom file %s\n", fftwPlanFileName);
            if(useWisdom)
                printf("fftw wisdom file=%s\n", fftwPlanFileName);
        }

        if(strcmp(argv[k],"-b")==0){
            if(!(beatFile = new ifstream(argv[k+1])) || beatFile->fail()){
                printf("Cannot open beat file %s\n", argv[k+1]);
                exit(1);
            }
            else
                beats=1;
        }

        if(strcmp(argv[k],"-l")==0){
            loEdge=atof(argv[k+1]);
            if(loEdge<0 || loEdge>8000.0)
                error("loEdge is out of range (0 - 8kHz)");
        }
        if(strcmp(argv[k],"-i")==0){
            hiEdge=atof(argv[k+1]);
            if(hiEdge<loEdge || hiEdge>22050.0)
                error("hiEdge is out of range (0 - 22.05kHz)");
        }
        if(strcmp(argv[k],"-n")==0 and !(usingS)){
            fftN = atoi(argv[k+1]);
        }
        if(strcmp(argv[k],"-w")==0 and !(usingS)){
            winN = atoi(argv[k+1]);
        }
        if(strcmp(argv[k],"-h")==0 and !(usingS)){
            hopN = atoi(argv[k+1]);
        }
        if(strcmp(argv[k], "-s")==0){
            usingS = true;
            hopTime = atoi(argv[k+1]);
        }
        if(strcmp(argv[k],"-v")==0){
            verbose = atoi(argv[k+1]);
        }
        if(strcmp(argv[k],"-C")==0){
            extractChannel = atoi(argv[k+1]);
            if(extractChannel<0 || extractChannel>128){
                printf("Badly formed extract channel: %d", extractChannel);
                exit(1);
            }
        }
        if(strcmp(argv[k],"-q")==0){
            if(!constantQ)
                constantQ = atoi(argv[k+1]);
        }
        if(strcmp(argv[k],"-c")==0){
            chromagram = atoi(argv[k+1]);
            if(chromagram){ // Default Chromagram
                if(constantQ && verbose){
                    printf("Warning: overriding %d constantQ bands for %d chromagram bins\n", constantQ, chromagram);
                    fflush(stdout);
                }
                constantQ=chromagram;
            }
        }
        if(strcmp(argv[k],"-m")==0){
            cepstrum = atoi(argv[k+1]);
            if(cepstrum && !constantQ)
                constantQ=12;
        }
    }

    // Warn if winN<hopN
    if(winN<hopN)
        printf("Warning: hopSize %d is greater than windowLength %d\n, setting windowLength to hopSize %d", hopN, winN, hopN);
    // The hop is major, set winN to be at least hopN
    if(winN<hopN){
        winN = hopN;
    }

    if(loEdge==0.0){
        loEdge = 55.0 * pow(2.0, 2.5/12.0); // low C minus quater tone
    }
    if(hiEdge==0.0){
        hiEdge=8000.0;
    }
    if (!constantQ && !cepstrum && !chromagram){
        //if no feature is given, do a cqt
        constantQ = DEFAULT_FEAT_BANDS;
    }
    if (((!winN || !hopN || !fftN) && !hopTime) && !beats){
        //either specify (winN, hopN, fftN) or hopTime or use beats
        //if neither of these three, use the default hopTime
        hopTime = DEFAULT_SLICE_SIZE;
        usingS = true;
        if (winN || hopN || fftN){
            printf("Warning: If using sample specific fft size, window size, and hop size all must be specified.\n");
            printf("\tAt least on of the three was missing.  Proceeding with the default slicing of %ims.\n",hopTime);
        }
    } else if (((!winN || !hopN || !fftN) || !hopTime) && beats){
        //for beat features use a hop of 100ms unless overridden
        hopTime = 100;
    }
    if(verbose) {
        if (power){
            printf("Overall power\n");
        }
        if (harmonicity){
            printf("Overall harmonicity\n");
        }
        if(constantQ && !(chromagram || cepstrum)){
            printf("Constant Q transform : %d bands per octave\n", constantQ);
        }
        if(chromagram){
            printf("Chromagram transform : %d chroma bins\n", chromagram );
        }
        if(cepstrum){
            printf("Cepstral transform : %d cepstral coefficients\n", cepstrum );
        }
        if(chromagram || constantQ || cepstrum){
            if(loEdge!=0.0){
                printf("loEdge: %f\n", loEdge);
            }
            if(hiEdge!=0.0){
                printf("hiEdge: %f\n", hiEdge);
            }
        }
        if (usingS)
            printf("creating sample rate invarent feature slices of length %ims.\n", hopTime);
        else
            printf("fftN=%d, winN=%d, hopN=%d\n", fftN, winN, hopN);
        if(beats){
            printf("Beat synchronus features\n");
        }
        fflush(stdout);
    }
}

void ConstantQTransform::extract()
{ // beat synchronus feature extraction
    double beati,beatj,t=0.0;
    double fileLength = sfinfo.frames/(double)sfinfo.samplerate;
    // Initialize extraction by reading a window's worth of audio
    if(verbose){
        cout << "extracting..." << endl;
        cout.flush();
    }
    outFile->write( (char*) &extractN, sizeof(int) ); // Feature dimension
    if(beats){
        *beatFile >> beati;
        *beatFile >> beatj;
        if(beatj<=beati)
            error("First two beats are not a sequence.\n");
        if(beati > fileLength)
            error("First beat is past end of file\n");
        if(beatj > fileLength){
            beatj=fileLength;
            std::cerr << "Setting second beat to file length: " << fileLength << std::endl;
        }
        readCount=0;
        while(readCount<(unsigned int)floor(beati*sfinfo.samplerate))
            readCount += sf_readf_double (inFile, audioData, min((unsigned int)winN,(unsigned int)floor(beati*sfinfo.samplerate)-readCount));
        if(readCount<(unsigned int)floor(beati*sfinfo.samplerate)){
            printf("Error reading audioData from file : read %d/%d\n", readCount, winN);
            exit(1);
        }
        t = beati;
    }
    readCount = sf_readf_double (inFile, audioData, winN);
    if(readCount<(unsigned int) winN){
        printf("Error reading audioData from file : read %d/%d\n", readCount, winN);
        exit(1);
    }
    // Extractor loop
    do{
        // process audio frames to FFT buffer
        windowAudioData();
        if (power) {
            powerOut[0] = 0;
            for(int k = 0; k < winN; k++) {
                powerOut[0] += in[k] * in[k];
            }
            powerOut[0] = log10(powerOut[0] / winN);
        } else if (harmonicity) {
        /* A quick comment about implementation choices here: we are
        looking for a "fundamental" frequency within a certain
        range.  We arbitrarily decide that that range is between
        the loEdge or twice the window frequency, whichever is
        greater, and one quarter of the hiEdge frequency. */
        int loN = ceil(sfinfo.samplerate / loEdge);
        if (loN > winN / 2) {
            loN = winN / 2;
        }
        int hiN = floor(sfinfo.samplerate / (hiEdge/4));
        double max = 0.0;
        for(int lag = hiN; lag < loN; lag++) {
            double current = 0.0;
            for(int k = 0; k < winN - lag; k++) {
                current += in[k] * in[k+lag];
            }
            current = current / (winN - lag);
            if(current > max) {
                max = current;
            }
        }
        harmonicityOut[0] = log10(max);
        } else {
        // Compute FFT
        fftw_execute(p);
        // Extract power spectrum
        extractFeatures();
        }
        if(beats){
            if(t + hopN/(double)sfinfo.samplerate >=beatj || t >= fileLength - winN/(double)sfinfo.samplerate){
                double interval = (beatj - t) / (beatj - beati);
                beatSum(extractedData, beatSynchData, interval);       // Sum
                beatDiv(beatSynchData, (beatj-beati)*sfinfo.samplerate/(double)hopN); // Average
                outFile->write( (const char*) beatSynchData, extractN * sizeof(double) );
                beati = beatj;
                *beatFile >> beatj;
                newBeat(extractedData, beatSynchData, 1 - interval);
            }
            else
                beatSum(extractedData, beatSynchData, 1.0); // Sum
        } else
            // Write output
            outFile->write( (const char*) extractedData, extractN * sizeof(double) );
        // Perform buffer update
        updateBuffers();
        t+=hopN/(double)sfinfo.samplerate; // current frame time
        // Read next window of audioData
        readCount = sf_readf_double(inFile, audioData+(winN-hopN)*sfinfo.channels, hopN);
    } while( readCount>0 &&  ! ( beats && beatFile->eof() ) );
    outFile->flush();
    outFile->close();
}

void ConstantQTransform::makeHammingWindow(){
    hammingWindow = new double[winN];
    for(int i=0; i < winN; i++){
      hammingWindow[i] = 0.54 - 0.46*cos(M_TWOPI*i/(winN-1));
    }
}

void ConstantQTransform::extractConstantQ(){
    int a,b,i;
    double *ptr1, *ptr2, *ptr3;
    // matrix product of CQT * FFT
    a=cqtN;
    i=0;
    while( a-- ){
        ptr1=cqtOut+i;
        *ptr1=0.0;
        ptr2 = CQT+i++*outN;
        ptr3 = fftOut;
        b=outN;
        while(b--)
            *ptr1+=*ptr2++**ptr3++;
        ptr1++;
    }

    // Chromagram and Cepstrum are mututally exclusive
    // CHROM ( in-place )
    if( chromagram ){
        a=constantQ; // Number of CQ bands in one octave
        ptr1 = cqtOut;
        i=0;
        while(a--){
            int c = 1;
            for(b=constantQ+i; b<cqtN; b+=constantQ){
                *ptr1+=cqtOut[b];
                c++;
            }
            *ptr1 /= c; // Normalize by number of bands summed
            *ptr1 = log10( *ptr1 );
            ptr1++;
            i++;
        }
    // LFCC ( out-of-place )
    // Want coefficients 2 to cqtN+1 as 1 to cqtN
    } else if( cepstrum ){
        // copy CQT values to tmp array
        double* tmp = new double[cqtN];
        assert( tmp && dctN && ( dctN <= cqtN ) );
        a = cqtN;
        ptr1 = cqtOut;
        ptr2 = tmp;
        while( a-- )
            *ptr2++ = log10( *ptr1++ );
        a = dctN;
        ptr2 = DCT+cqtN*3; // point to 4th column of DCT
        ptr3 = cqtOut;
        while( a-- ){
            ptr1 = tmp;  // point to cqt vector copy
            *ptr3 = 0.0;
            b = cqtN;
            while( b-- )
                *ptr3 += *ptr1++ * *ptr2++;
            ptr3++;
        }
        delete[] tmp; // free up temporary memory
    // PUT CQT ON LOG SCALE
    }else{
        a=cqtN;
        ptr1 = cqtOut;
        while( a-- ){
            *ptr1 = log10 ( *ptr1 );
            ptr1++;
        }
    }
}
