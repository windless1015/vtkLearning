#pragma once

#include <QWidget>
#include "QVTKOpenGLNativeWidget.h"               //新版本，旧版QVTKWidget

#include "vtkAutoInit.h"
#include "vtkContextItem.h"
#include <vtkCommand.h>
#include "vtkMinimalStandardRandomSequence.h"
#include "vtkVariant.h"
#include "vtkContext2D.h"
#include "vtkContextScene.h"
#include <vtkTextProperty.h>
#include <vtkPen.h>
#include <vtkBrush.h>

#define vSP vtkSmartPointer    
#define vSPNew(Var, Type)    vSP<Type> Var = vSP<Type>::New(); 

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingContextOpenGL2);



class MyPickCallback : public vtkCommand
{
public:
    static MyPickCallback* New()
    {
        return new MyPickCallback;
    }

    virtual void Execute(vtkObject* caller, unsigned long eventId, void* vtkNotUsed(callData))
    {
    	if (eventId == vtkCommand::PickEvent)
    	{
    		/*vtkRenderer* renderer = vtkRenderer::SafeDownCast(caller);
    		if (renderer)
    		{
    			std::cout << "Actor picked!" << std::endl;
    		}*/
    	}
    }
};





class vtkContext2D;
class vtkActor;
class vtkRenderer;

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

class FollowerIPRWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FollowerIPRWidget(QWidget* parent = nullptr);
    ~FollowerIPRWidget();
    void readSTLFile(QString stlFilePath);
private:
    QVTKOpenGLNativeWidget* openGLWidget;
    vSP<vtkRenderer> renderer;
    vSP<vtkActor> m_stlActor;
};

