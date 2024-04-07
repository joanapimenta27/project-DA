#include <iostream>
#include "Interface.h"
#include "Management.h"
#include "utils.h"

int main() {
    //Interface anInterface;
    //anInterface.run();
    Management man(0);
    man.balanceBasicMetrics(*man.getWaterNetwork());
}
