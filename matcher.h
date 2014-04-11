#ifndef MATCHER_H
#define MATCHER_H

#include "database.h"
#include "pairofpeaks.h"
#include <vector>

class Matcher
{
public:
    Matcher(Database<pair<string, int > >* db);
    void findMatches(vector< PairOfPeaks*> &peaks);
private:
    Database<pair<string, int > >* database;
};

#endif // MATCHER_H
