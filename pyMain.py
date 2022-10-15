
import clr
clr.AddReference("D:\\AutoDisk\\Revit Build\\Excutable\\Product\\Revit 2023\\RevitAPI.dll")

from Autodesk.Revit.DB import Transaction, WorksetKind, FilteredWorksetCollector
from Autodesk.Revit.DB import WorksetDefaultVisibilitySettings

doc = __revit__.ActiveUIDocument.Document

# collect all worksets
worksets = FilteredWorksetCollector(doc).OfKind(WorksetKind.UserWorkset)

# iterate over  worksets
for workset in worksets:
    # Find workset you want to target by name
    if workset.Name == 'Replace this with Name of Workset to Target':
        t = Transaction(doc)
        t.Start('Hide Workset in all Views')

        defaultVisibility = WorksetDefaultVisibilitySettings.GetWorksetDefaultVisibilitySettings(doc)
        defaultVisibility.SetWorksetVisibility(workset.Id, False)

        t.Commit()
