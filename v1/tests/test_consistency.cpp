#include "logic.h"

int testHandConsistency() {
    int rc = 0;
    Hand::forAllHands([&](Hand hand1) {
        Hand::forAllHands([&](Hand hand2) {
            bool io = (hand1.id() < hand2.id());
            bool ho = (hand1.hash() < hand2.hash());
            if(io != ho) rc = 1;
        });
    });
    return rc;
}

int main() {
    return testHandConsistency();
}