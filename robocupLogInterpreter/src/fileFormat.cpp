#include "fileFormat.h"
fileFormat fileFormatUserInstancer()
{
    fileFormat option;
    std::vector<std::string> alternatives;

    alternatives.reserve(3);
    alternatives.push_back("by game");
    alternatives.push_back("by teams");
    alternatives.push_back("single file");
    option.groupBy = multipleAlternativeQuestion("1. How do you wish to group data in files?", alternatives);
    if (option.groupBy == FF_GROUPBY_TEAMS)
        std::cout << "    WARNING: Since ball has no team organizing per Team ignores ball data!" << std::endl;

    alternatives.clear();
    alternatives.push_back("finalization plays");
    alternatives.push_back("finalization plays with only the offensive team");
    option.filterBy = multipleAlternativeQuestion("2. Wich data do you want to gather?", alternatives);

    int separator;
    alternatives.clear();
    alternatives.push_back("coma ', '");
    alternatives.push_back("blank space ' '");
    separator = multipleAlternativeQuestion("3. Wich CSV Separator do you want to use?", alternatives);
    if (separator == FF_SEPARATOR_COMMA)
        option.separator = ", ";
    if (separator == FF_SEPARATOR_BLANK)
        option.separator = " ";

    option.header = booleanQuestion("4. Do you want Headers on the file?");
    option.rowId = booleanQuestion("5. Do you want row IDs on the file?");
    option.cycle = booleanQuestion("6. Do you want a first cycle column on the File?");
    option.team = booleanQuestion("7. Do you want a team column on the File?");
    option.player = booleanQuestion("8. Do you want a player column on the File?");
    option.playLength = booleanQuestion("9. Do you want plays length column on the File?");
    option.halfField = booleanQuestion("10. Do you want to Annalize offenses in a single direction?");
    return option;
}
