#include "nodeinterface.h"

NodeInterface::NodeInterface()
{
    NumInputs = 0;
    NumOutputs = 0;
}
//--------------------------------------------------------------------------------------------------
void NodeInterface::SetNodeInputAttr(int index, QString ty, QString defaultval, QString mand, QString tipt, QString helpt)
{
    InputPort s;
    if((Inputs.length()==0) || (Inputs.length() < (index+1)))
    {
        s.type = ty;
        s.defaultvalue = defaultval;
        s.mandatory = (mand.toLower()=="true");
        s.tooltip = tipt;
        s.helptext = helpt;
        Inputs.append(s);
    }
    else // the input port attributes exist
    {
        s.type = ty;
        s.defaultvalue = defaultval;
        s.mandatory = (mand.toLower()=="true");
        s.tooltip = tipt;
        s.helptext = helpt;
        Inputs.replace(index, s);
    }
}
//--------------------------------------------------------------------------------------------------
void NodeInterface::SetNodeOutputAttr(int index, QString ty, QString defaultval, QString mand, QString tipt, QString helpt)
{
    OutputPort s;
    if((Outputs.length()==0) || (Outputs.length() < (index+1)))
    {
        s.type = ty;
        s.defaultvalue = defaultval;
        s.mandatory = (mand.toLower()=="true");
        s.tooltip = tipt;
        s.helptext = helpt;
        Outputs.append(s);
    }
    else // the input port attributes exist
    {
        s.type = ty;
        s.defaultvalue = defaultval;
        s.mandatory = (mand.toLower()=="true");
        s.tooltip = tipt;
        s.helptext = helpt;
        Outputs.replace(index, s);
    }
}
