#include "matcher.h"
#include "histogram.h"
#include <cmath>

Matcher::Matcher(Database<pair<string, int > >* db)
{
    database = db;
    db->readFromDisk();
}

void Matcher::findMatches(vector<PairOfPeaks *> &peaks){

    map<string, Histogram> possibleMatches;

    //loop alle peaks
    for(vector< PairOfPeaks*>::iterator it = peaks.begin(); it != peaks.end(); it++){

        //check of er peaks overeen komen in de database
        if(database->count((*it)->getHash())){

            //alle mogelijke tracks die deze pairofpeaks bevatten
            vector< pair< string, int > > results = database->find((*it)->getHash());

            //loop alle resultaten en verwerk ze in een histogram
            for(vector< pair< string, int > >::iterator it2 = results.begin(); it2!=results.end(); it2++){
                Histogram h;
                possibleMatches.insert(make_pair(it2->first, h));
                possibleMatches[it2->first].addValue(abs(it2->second - (*it)->getTime()));
            }
        }
    }

    multimap<int,string> orderedMatches;
    for(map<string, Histogram>::iterator it = possibleMatches.begin(); it!=possibleMatches.end();it++){
        orderedMatches.insert(make_pair(it->second.getMaxValue(),it->first));
    }

    for(multimap<int, string>::iterator it = orderedMatches.begin(); it!=orderedMatches.end();it++){
        cout << it->second << "=> count: " << it->first << endl;
    }
}
