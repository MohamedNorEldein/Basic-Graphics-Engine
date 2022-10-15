#include "pch.h"

#include "RevitApi.h"

#include <iostream>
#include "pch.h"
#include "App.h"
// HelloRevitCpp.cpp - this is the main DLL file.

using namespace System;
using namespace Autodesk::Revit::ApplicationServices;
using namespace Autodesk::Revit::Attributes;
using namespace Autodesk::Revit::DB;
using namespace Autodesk::Revit::UI;

using namespace HelloRevitCpp;


Result Command::Execute(
	ExternalCommandData^ commandData,
	String^% message,
	ElementSet^ elements)
{
	//main();

	return Autodesk::Revit::UI::Result::Succeeded;
}