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
    caption2D->BorderOff(); //�����߿���ʧ
    caption2D->GetTextActor()->SetTextScaleModeToViewport(); //�����ı��Ĵ�С
    caption2D->GetCaptionTextProperty()->SetFontSize(10);
    caption2D->SetPosition(100, 0);
    //caption2D->SetPosition2(200, 20);

    caption2D->GetProperty()->SetColor(0, 0, 1); //vtkCaptionActor2D Դ������,3dָʾ�ߵ���ɫ�Ǹ������������õ�
    caption2D->GetProperty()->SetLineWidth(1);

    caption2D->GetCaptionTextProperty()->FrameOn(); // �� text frame
    caption2D->GetCaptionTextProperty()->SetFrameWidth(1);// �߿�Ŀ��Ϊ1������

    if (false) //���������ֱ�ѡ��
    {
        caption2D->GetCaptionTextProperty()->SetColor(1.0f, 1.0f, 1.0f); //ipr�����ɫ
        caption2D->GetCaptionTextProperty()->SetBackgroundColor(0., 0., 1.0f); //����Ϊ��ɫ
        caption2D->GetCaptionTextProperty()->SetBackgroundOpacity(0.9);
    }
    else
    {
        caption2D->GetCaptionTextProperty()->SetColor(0, 0, 0); //ipr�����ɫ
    }

    caption2D->GetCaptionTextProperty()->SetFrameColor(0., 0., 1.0f);

    caption2D->GetCaptionTextProperty()->SetFontFamily(2);//2������ȽϺ���
    caption2D->GetCaptionTextProperty()->ShadowOff(); //�ر�������Ӱ,����������Ӱ
    caption2D->SetPadding(0);
    caption2D->SetThreeDimensionalLeader(true); //ָʾ��Ϊ��ά,������ʱ��ᱻ�ڵ�
    caption2D->LeaderOn(); //ָʾ�߿�
    caption2D->SetPickable(true);
    caption2D->SetAttachmentPoint(30.0, 0.0, 2.0); // caption2d ���ڵ�
   
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

