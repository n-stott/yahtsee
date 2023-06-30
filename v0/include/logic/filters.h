#ifndef FILTERS_H
#define FILTERS_H

template<typename It, typename Comp>
static inline It filterDominated(It begin1, It end1, It begin2, Comp comp) {
    for(It it = begin1; it != end1; ++it) {
        bool dominated = false;
        for(It it2 = begin1; it2 != end1; ++it2) {
            if(it == it2) continue;
            dominated |= comp(it, it2);
            if(dominated) break;
        }
        if(!dominated) {
            *begin2++ = *it;
        }
    }
    return begin2;
}


#endif