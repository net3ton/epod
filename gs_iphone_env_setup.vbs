Set objShell = WScript.CreateObject("WScript.Shell")
Set objFSO = CreateObject("Scripting.FileSystemObject") 
Set env_vars = objShell.Environment("User")

root = objFSO.GetParentFolderName(WScript.ScriptFullName)
env_vars("GS_IPHONE_ROOT") = root
env_vars("GS_IPHONE_PYTHONPATH") = root +"\"+ "py_libs"
python_path = env_vars("PYTHONPATH")
If 0 = InStr(python_path, "%GS_IPHONE_PYTHONPATH%") Then
  env_vars("PYTHONPATH") = "%GS_IPHONE_PYTHONPATH%;" + python_path
End If
