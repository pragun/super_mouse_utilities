## Notes ##
1. The visual studio project uses a PropertySheet (PropertySheet.props) to set the $(SCITERSDKPATH) variable.
The only place in the project this variable is not used is in the relative path of sciter-win-main.cpp.
The path for this file needs to be set manuall to the place where SCITERSDK resides


