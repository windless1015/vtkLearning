#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkButtonWidget.h>
#include <vtkTexturedButtonRepresentation2D.h>
#include <vtkCoordinate.h>
#include <vtkImageData.h>

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

using namespace std;

enum ButtonType{ E_Button0, E_Button1, E_ButtonCount };

vtkSPtr<vtkButtonWidget> m_Button[E_ButtonCount];
vtkSPtr<vtkRenderWindowInteractor> m_RenderWindowInteractor;
vtkSPtr<vtkRenderer> m_Renderer;

void CreateImage(vtkSmartPointer<vtkImageData> image,
    unsigned char* color1,
    unsigned char* color2);

ButtonType GetButtonType( const vtkButtonWidget *const buttonWidget )
{
    ButtonType result = E_ButtonCount;
    for( int i = 0; i < E_ButtonCount; ++i )
    {
        if( buttonWidget == m_Button[i] )
        {
            result = (ButtonType)i;
            break;
        }
    }
    return result;
}

class ButtonCallBack: public vtkCommand
{
public:
    static ButtonCallBack *New(){ return new ButtonCallBack(); }
    virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData)
    {
        vtkButtonWidget* buttonWidget = reinterpret_cast<vtkButtonWidget*>(caller);
        vtkTexturedButtonRepresentation2D* rep = reinterpret_cast<vtkTexturedButtonRepresentation2D*>(buttonWidget->GetRepresentation());
        int state = rep->GetState();
        ButtonType buttonT = GetButtonType( buttonWidget );
        cout << "state: " << state << ", buttonT: " << buttonT << endl;
        // add your costum click function for different button.
    }
};

void InitButton(const ButtonType buttonType, const std::string& beforeImagePath, const std::string& afterImagePath)
{
   /* vtkSmartPointer<vtkPNGReader>
    vtkSPtrNew(pngReader, vtkPNGReader);
    pngReader->SetFileName( beforeImagePath.toLocal8Bit().data() );
    pngReader->Update();

    vtkSPtrNew(pngReader1, vtkPNGReader);
    pngReader1->SetFileName( afterImagePath.toLocal8Bit().data() );
    pngReader1->Update();*/

    vtkSmartPointer<vtkImageData> image1 =
        vtkSmartPointer<vtkImageData>::New();
    vtkSmartPointer<vtkImageData> image2 =
        vtkSmartPointer<vtkImageData>::New();
    unsigned char banana[3] = { 227, 207, 87 };
    unsigned char tomato[3] = { 255, 99, 71 };
    CreateImage(image1, banana, tomato);
    CreateImage(image2, tomato, banana);

    vtkSPtrNew( buttonRep, vtkTexturedButtonRepresentation2D );
    if( beforeImagePath != afterImagePath )
    {
        buttonRep->SetNumberOfStates( 2 );
        buttonRep->SetButtonTexture( 0, image1);
        buttonRep->SetButtonTexture( 1, image2);
    }
    else
    {
        buttonRep->SetNumberOfStates( 1 );
        buttonRep->SetButtonTexture( 0, image2);
    }

    m_Button[buttonType]->SetInteractor( m_RenderWindowInteractor );
    m_Button[buttonType]->SetRepresentation( buttonRep );

    buttonRep->SetPlaceFactor( 1 );
    m_Button[buttonType]->On();

    vtkSPtrNew(buttonCB, ButtonCallBack);
    m_Button[buttonType]->AddObserver( vtkCommand::StateChangedEvent, buttonCB );
}

void UpdateButtonsPosition()
{
    vtkWidgetRepresentation* buttonRep = m_Button[E_Button0]->GetRepresentation();

    vtkSPtrNew(coordinate, vtkCoordinate);
    coordinate->SetCoordinateSystemToNormalizedDisplay();
    coordinate->SetValue(1.0, 0.8);
    double bds[6];
    double sz = 50;
    bds[0] = coordinate->GetComputedDisplayValue(m_Renderer)[0] - sz;
    bds[1] = bds[0] + sz;
    bds[2] = coordinate->GetComputedDisplayValue(m_Renderer)[1] - sz;
    bds[3] = bds[2] + sz;
    bds[4] = bds[5] = 0;
    buttonRep->PlaceWidget(bds);

    // ================== Put the second button ==================
    double tmpBds[6] = { bds[0], bds[1], bds[2], bds[3], bds[4], bds[5] };
    buttonRep = m_Button[E_Button1]->GetRepresentation();
    bds[0] = tmpBds[0];
    bds[1] = bds[0] + sz;
    bds[2] = tmpBds[2] - sz - 5;
    bds[3] = bds[2] + sz;
    bds[4] = bds[5] = 0;
    buttonRep->PlaceWidget( bds );
}

int main()
{
    vtkSPtrNew( cone, vtkConeSource );
    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputConnection( cone->GetOutputPort() );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    m_Renderer = vtkSPtr<vtkRenderer>::New();
    m_Renderer->AddActor(actor);
    m_Renderer->SetBackground( 0, 0, 0 );

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( m_Renderer );

    m_RenderWindowInteractor = vtkSPtr<vtkRenderWindowInteractor>::New();
    m_RenderWindowInteractor->SetRenderWindow( renderWindow );

    for( int i = 0; i < 2; ++i )
    {
        m_Button[i] = vtkSmartPointer<vtkButtonWidget>::New();
    }
    std::string path0 = "/Users/weiyang/Desktop/1.png";
    std::string path1 = "/Users/weiyang/Desktop/2.png";
    InitButton( E_Button0, path0, path1 );
    InitButton( E_Button1, path1, path0 );

    m_Renderer->ResetCamera();
    renderWindow->Render();

    UpdateButtonsPosition();

    m_RenderWindowInteractor->Start();
    return 0;
}

void CreateImage(vtkSmartPointer<vtkImageData> image,
    unsigned char* color1,
    unsigned char* color2)
{
    // Specify the size of the image data
    image->SetDimensions(10, 10, 1);
#if VTK_MAJOR_VERSION <= 5
    image->SetNumberOfScalarComponents(3);
    image->SetScalarTypeToUnsignedChar();
#else
    image->AllocateScalars(VTK_UNSIGNED_CHAR, 3);
#endif
    int* dims = image->GetDimensions();

    // Fill the image with
    for (int y = 0; y < dims[1]; y++)
    {
        for (int x = 0; x < dims[0]; x++)
        {
            unsigned char* pixel =
                static_cast<unsigned char*>(image->GetScalarPointer(x, y, 0));
            if (x < 5)
            {
                pixel[0] = color1[0];
                pixel[1] = color1[1];
                pixel[2] = color1[2];
            }
            else
            {
                pixel[0] = color2[0];
                pixel[1] = color2[1];
                pixel[2] = color2[2];
            }
        }
    }
}