#include "followerIPRWidget.h"
#include <vtkFollower.h>
#include <vtkTextActor.h>
#include <vtkProperty.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkMapper.h>
#include <vtkMapper2D.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkVectorText.h>
#include <vtkContextActor.h>
#include <vtkBillboardTextActor3D.h>
#include <vtkSTLReader.h>
#include <vtkCaptionActor2D.h>
#include <vtkCaptionRepresentation.h>
#include <vtkCaptionWidget.h>
#include <vtkCamera.h>
#include <vtkProperty2D.h>

FollowerIPRWidget::FollowerIPRWidget(QWidget* parent) :
    QWidget(parent)
{
    vtkNew<vtkNamedColors> colors;
    renderer = vSP<vtkRenderer>::New();

    renderer->SetBackground(colors->GetColor3d("PaleTurquoise").GetData()); //LightGrey  WhiteSmoke  PaleTurquoise
    openGLWidget = new QVTKOpenGLNativeWidget(this);
    openGLWidget->resize(800, 500);
    openGLWidget->renderWindow()->AddRenderer(renderer);


    vtkNew<vtkCaptionActor2D > caption2D;
    caption2D->SetCaption("0.2");
    caption2D->BorderOff(); //最外层边框消失
    caption2D->GetTextActor()->SetTextScaleModeToViewport(); //调节文本的大小
    caption2D->GetCaptionTextProperty()->SetFontSize(10);
    caption2D->SetPosition(100, 0);
    //caption2D->SetPosition2(200, 20);

    caption2D->GetProperty()->SetColor(0, 0, 1); //vtkCaptionActor2D 源代码中,3d指示线的颜色是根据其自身设置的
    caption2D->GetProperty()->SetLineWidth(1);

    caption2D->GetCaptionTextProperty()->FrameOn(); // 打开 text frame
    caption2D->GetCaptionTextProperty()->SetFrameWidth(1);// 边框的宽度为1个像素

    if (false) //如果这个数字被选中
    {
        caption2D->GetCaptionTextProperty()->SetColor(1.0f, 1.0f, 1.0f); //ipr字体白色
        caption2D->GetCaptionTextProperty()->SetBackgroundColor(0., 0., 1.0f); //背景为蓝色
        caption2D->GetCaptionTextProperty()->SetBackgroundOpacity(0.9);
    }
    else
    {
        caption2D->GetCaptionTextProperty()->SetColor(0, 0, 0); //ipr字体黑色
    }

    caption2D->GetCaptionTextProperty()->SetFrameColor(0., 0., 1.0f);

    caption2D->GetCaptionTextProperty()->SetFontFamily(2);//2号字体比较合适
    caption2D->GetCaptionTextProperty()->ShadowOff(); //关闭数字阴影,否则会出现重影
    caption2D->SetPadding(0);
    caption2D->SetThreeDimensionalLeader(true); //指示线为三维,否则有时候会被遮挡
    caption2D->LeaderOn(); //指示线开
    caption2D->SetPickable(true);
    caption2D->SetAttachmentPoint(30.0, 0.0, 2.0); // caption2d 所在的
   
    renderer->AddActor(caption2D);

    vtkSmartPointer<MyPickCallback> pickCallback = vtkSmartPointer<MyPickCallback>::New();
    renderer->AddObserver(vtkCommand::PickEvent, pickCallback);

}

FollowerIPRWidget::~FollowerIPRWidget()
{

}

void FollowerIPRWidget::readSTLFile(QString stlFilePath)
{
    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkSTLReader> reader;
    reader->SetFileName(stlFilePath.toStdString().c_str());
    reader->Update();

    // Visualize
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(reader->GetOutputPort());

    m_stlActor = vSP<vtkActor>::New();
    m_stlActor->SetMapper(mapper);
    m_stlActor->GetProperty()->SetDiffuse(0.8);
    m_stlActor->GetProperty()->SetDiffuseColor(
        colors->GetColor3d("LightSteelBlue").GetData());
    m_stlActor->GetProperty()->SetSpecular(0.3);
    m_stlActor->GetProperty()->SetSpecularPower(60.0);

    renderer->AddActor(m_stlActor);
}

