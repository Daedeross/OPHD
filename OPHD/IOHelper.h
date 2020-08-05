#pragma once

#include <NAS2D/Xml/Xml.h>
#include <string>

struct StorableResources;

void readResources(NAS2D::Xml::XmlElement* element, StorableResources& resources);

void writeResources(NAS2D::Xml::XmlElement*, const StorableResources&, const std::string&);
