#include <sequanto/QtStatsProperties.h>
#include <sequanto/QtAutomationGetPropertyEvent.h>
#include <sequanto/QtCache.h>

using namespace sequanto::automation;

QtStatsGetPropertyEventsHandled::QtStatsGetPropertyEventsHandled()
   : ReadOnlyIntegerPropertyNode("get_property_events_handled")
{
}

int QtStatsGetPropertyEventsHandled::GetValue ()
{
   return QtAutomationGetPropertyEvent::eventsPosted();
}

QtStatsGetPropertyAverageDeliveryTime::QtStatsGetPropertyAverageDeliveryTime()
   : ReadOnlyFloatPropertyNode("get_property_average_delivery_time")
{
}

float QtStatsGetPropertyAverageDeliveryTime::GetValue ()
{
   return QtAutomationGetPropertyEvent::averageDeliveryTime();
}

QtStatsCacheHits::QtStatsCacheHits()
   : ReadOnlyIntegerPropertyNode("cache_hits")
{
}

int QtStatsCacheHits::GetValue ()
{
   return QtCache::Instance().cacheHits();
}

QtStatsCacheMisses::QtStatsCacheMisses()
   : ReadOnlyIntegerPropertyNode("cache_misses")
{
}

int QtStatsCacheMisses::GetValue ()
{
   return QtCache::Instance().cacheMisses();
}
