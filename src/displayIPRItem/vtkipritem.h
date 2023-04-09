#ifndef VTKGRAPHITEM_H
#define VTKGRAPHITEM_H

#include "QVTKOpenGLNativeWidget.h"               //新版本，旧版QVTKWidget
#include "vtkAutoInit.h"
#include "vtkContextItem.h"
#include "vtkEdgeListIterator.h"
#include "vtkOutEdgeIterator.h"
#include "vtkGraph.h"
#include "vtkMinimalStandardRandomSequence.h"
#include "vtkVariant.h"
#include "vtkContext2D.h"
#include "vtkContextScene.h"
#include "vtkContextMouseEvent.h"
#include "vtkPen.h"
#include "vtkBrush.h"
#include "vtkTextProperty.h"
#include "vtkTransform2D.h"
#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include "utility"
#include "vector"

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingContextOpenGL2);

class vtkContext2D;
class vtkGraph;

const int iprSquareEdge = 40;

class MyDrawing : public vtkContextItem
{
public:
    vtkTypeMacro(MyDrawing, vtkContextItem);
    static MyDrawing* New();
    bool SetIsSelected();

    virtual bool Paint(vtkContext2D* painter) override
    {
        painter->GetTextProp()->SetVerticalJustificationToCentered();
        painter->GetTextProp()->SetJustificationToCentered();
        painter->GetTextProp()->SetFontSize(20); //设置方框内字体的大小
        painter->GetPen()->SetColor(0, 0, 255);//边缘是蓝色
        
        if (false) //选中
        {
            painter->GetTextProp()->SetColor(1.0f, 1.0f, 1.0f); //白色字体
            painter->GetBrush()->SetColor(0, 0, 255); //蓝色背景
        }
        else  //未选中
        {
            painter->GetTextProp()->SetColor(0.0f, 0.0f, 0.0f); //黑色字体
            painter->GetBrush()->SetColor(255, 255, 255); //白色背景
        }

        painter->DrawRect(20, 20, iprSquareEdge, iprSquareEdge);
        painter->DrawString(iprSquareEdge - 2, iprSquareEdge - 2, vtkVariant(0.2).ToString());
        return true;
    }
private:
    bool m_isSelected;

};



class vtkIPRItem : public vtkContextItem
{

public:
    vtkTypeMacro(vtkIPRItem, vtkContextItem);
    void PrintSelf(ostream& os, vtkIndent indent) override;
    static vtkIPRItem* New();
    vtkGetObjectMacro(Graph, vtkGraph);
    virtual void SetGraph(vtkGraph* g);


    bool Paint(vtkContext2D* painter) override;

    bool Hit(const vtkContextMouseEvent& mouse)override;

    bool MouseEnterEvent(const vtkContextMouseEvent& mouse)override;

    bool MouseMoveEvent(const vtkContextMouseEvent& mouse)override;

    bool MouseLeaveEvent(const vtkContextMouseEvent& mouse)override;

    bool MouseButtonPressEvent(const vtkContextMouseEvent& mouse)override;

    bool MouseButtonReleaseEvent(const vtkContextMouseEvent& mouse)override;

    void UpdatePositions();



protected:

    vtkIPRItem();
    ~vtkIPRItem() override;

    float LastPosition[2];
    bool MouseOver;
    int MouseButtonPressed;


    vtkGraph* Graph;
    vtkIdType HitVertex;

    class Implementation;

    Implementation* Impl;

private:
    vtkIPRItem(const vtkIPRItem&) = delete;
    void operator =(const vtkIPRItem&) = delete;

};

#endif // VTKGRAPHITEM_H
