#include "uniformobject.h"

int UniformObjectMonitor::currentBindingPoint = 0;

UniformObjectMonitor::UniformObjectMonitor()
{
    ownBindingPoint = currentBindingPoint;
    currentBindingPoint++;
}

UniformObjectMonitor::~UniformObjectMonitor()
{
}
