#pragma once
#include "../CivilEngine/headers/App.h"


using namespace System;
using namespace Autodesk::Revit::ApplicationServices;
using namespace Autodesk::Revit::Attributes;
using namespace Autodesk::Revit::DB;
using namespace Autodesk::Revit::UI;

using namespace System;

#pragma once
 
namespace HelloRevitCpp {


    [Autodesk::Revit::Attributes::Regeneration(
        Autodesk::Revit::Attributes::RegenerationOption::Manual)]

       [Autodesk::Revit::Attributes::Transaction(
          Autodesk::Revit::Attributes::TransactionMode::Manual)]

    public ref class Command
        : IExternalCommand
    {
    public:
        virtual Result Execute(
            ExternalCommandData^ commandData,
            String^% message,
            ElementSet^ elements);
    };
}