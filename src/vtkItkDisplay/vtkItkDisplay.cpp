//保证VTK正常运行
#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkGDCMImageIO.h"
#include "itkImageToVTKImageFilter.h"
#include "itkGDCMImageIOFactory.h"
#include "itkNrrdImageIOFactory.h"
#include "itkNiftiImageIOFactory.h"
#include "itkMINCImageIOFactory.h"
#include "itkMetaImageIOFactory.h"
#include "itkBMPImageIOFactory.h"
#include "itkPNGImageIOFactory.h"
#include "itkJPEGImageIOFactory.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkOrientImageFilter.h"
#include "itkMetaDataObject.h"
#include "itkImageFileReader.h"

#include <vtkSmartPointer.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageReslice.h>
#include <vtkImageViewer2.h>

bool test();
void renderVtkImageData(vtkSmartPointer<vtkImageData>& imageData);
int main(int argc, char * argv[])
{
    /*
    using PixelType = float;
    using ImageType = itk::Image<PixelType, 3>;
    //1. read the image from local file
	itk::ImageFileReader<ImageType>::Pointer reader = itk::ImageFileReader<ImageType>::New();
	reader->SetFileName(path);
	try {
		reader->Update();
	}
	catch (itk::ExceptionObject & err) {
		std::cout << "ERROR while parsing fixed volume: " << err.what() << std::endl;
		return false;
	}

	//2. OrientImageFilter change image orientation(direction)
	itk::OrientImageFilter<ImageType, ImageType>::Pointer orientationFilter = itk::OrientImageFilter<ImageType, ImageType>::New();
	orientationFilter->UseImageDirectionOn();
	orientationFilter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
	orientationFilter->SetInput(reader->GetOutput());
	try {
		orientationFilter->Update();
	}
	catch (itk::ExceptionObject & err) {
		std::cout << "ERROR while correcting fixed volume orientation: " << err.what() << std::endl;
		return false;
	}

	//3. get image that has been changed orientation
	itk::Image<float, 3U> *volumeImgPtr = orientationFilter->GetOutput();
    */

    //std::string filePath = "D:\\projects\\Fussen\\shangtaoSkull\\dicom\\0262.dcm";

    //using PixelType = signed short;
    //using ImageType = itk::Image<PixelType, 3>;
    ////1. read the image from local file
    //itk::ImageFileReader<ImageType>::Pointer reader = itk::ImageFileReader<ImageType>::New();
    //reader->SetFileName(filePath);
    //try {
    //    reader->Update();
    //}
    //catch (itk::ExceptionObject & err) {
    //    std::cout << "ERROR " << err.what() << std::endl;
    //    return false;
    //}

    //itk::OrientImageFilter<ImageType, ImageType>::Pointer orientationFilter = itk::OrientImageFilter<ImageType, ImageType>::New();
    //orientationFilter->UseImageDirectionOn();
    //orientationFilter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
    //orientationFilter->SetInput(reader->GetOutput());
    //try {
    //    orientationFilter->Update();
    //}
    //catch (itk::ExceptionObject & err) {
    //    std::cout << "ERROR while correcting fixed volume orientation: " << err.what() << std::endl;
    //    return false;
    //}

    ////3. get image that has been changed orientation
    ////itk::Image<PixelType, 3U> *volumeImgPtr = orientationFilter->GetOutput();

    ////创建读取DCM的GDCMIOImage类
    //using ImageIOType = itk::GDCMImageIO;
    //ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
    //reader->SetImageIO(gdcmImageIO);
    //try
    //{
    //    reader->Update();
    //}
    //catch (itk::ExceptionObject& e)
    //{
    //    std::cerr << "exception in file reader" << std::endl;
    //    std::cerr << e << std::endl;
    //    return EXIT_FAILURE;
    //}

    ////链接滤波器，转换为VTK类型
    //typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
    //ConnectorType::Pointer connector = ConnectorType::New();
    //connector->SetInput(reader->GetOutput());
    //try
    //{
    //    connector->Update();
    //}
    //catch (itk::ExceptionObject& e)
    //{
    //    std::cerr << "exception in file reader" << std::endl;
    //    std::cerr << e << std::endl;
    //    return EXIT_FAILURE;
    //}

    ///* vtkImageActor在3D场景下渲染图像 */
    //vtkSmartPointer<vtkImageActor> actor = vtkImageActor::New();
    //actor->SetInputData(connector->GetOutput());
    //actor->InterpolateOff();
    //actor->Update();


    //vtkSmartPointer<vtkRenderer> render = vtkRenderer::New();
    //render->AddActor(actor);
    //render->SetBackground(255, 1, 1);//设置窗口背景颜色

    //vtkSmartPointer<vtkRenderWindow> window = vtkRenderWindow::New();
    //window->SetSize(600, 600);       //设置窗口大小
    //window->AddRenderer(render);
    //window->SetWindowName("the CT picture ");//设置窗口名称

    //vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
    //interactor->SetRenderWindow(window);

    //interactor->Initialize();
    //interactor->Start();
    //return 0;


    bool r = test();
    return EXIT_SUCCESS;


}

bool test()
{
    itk::NrrdImageIOFactory::RegisterOneFactory();
    itk::NiftiImageIOFactory::RegisterOneFactory();
    itk::MINCImageIOFactory::RegisterOneFactory();
    itk::MetaImageIOFactory::RegisterOneFactory();
    itk::GDCMImageIOFactory::RegisterOneFactory();
    itk::BMPImageIOFactory::RegisterOneFactory();
    itk::PNGImageIOFactory::RegisterOneFactory();
    itk::JPEGImageIOFactory::RegisterOneFactory();


    //std::string filePath = "D:\\projects\\Fussen\\shangtaoSkull\\dicom\\0262.dcm";
    std::string filePath = "D:\\Libs64\\ultrast-libs\\testMHA2NRRD\\3D-USScan_1.mha";
    //std::string filePath = "D:\\Libs64\\ultrast-libs\\testMHA2NRRD\\Scan_1.nrrd";
    //std::string filePath = "D:\\projects\\Fussen\\shangtaoSkull\\dicom\\0262.mnc";

    using PixelType = signed short;
    using ImageType = itk::Image<PixelType, 3>;
    //1. read the image from local file
    itk::ImageFileReader<ImageType>::Pointer reader = itk::ImageFileReader<ImageType>::New();
    reader->SetFileName(filePath);
    try {
        reader->Update();
    }
    catch (itk::ExceptionObject & err) {
        std::cout << "ERROR " << err.what() << std::endl;
        return false;
    }

    itk::OrientImageFilter<ImageType, ImageType>::Pointer orientationFilter = itk::OrientImageFilter<ImageType, ImageType>::New();
    orientationFilter->UseImageDirectionOn();
    orientationFilter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
    orientationFilter->SetInput(reader->GetOutput());
    try {
        orientationFilter->Update();
    }
    catch (itk::ExceptionObject & err) {
        std::cout << "ERROR while correcting fixed volume orientation: " << err.what() << std::endl;
        return false;
    }

    // itkImage to vtkImageData
    typedef itk::ImageToVTKImageFilter<ImageType> itkToVtkFilter;
    itkToVtkFilter::Pointer itkToVtkConnector = itkToVtkFilter::New();
    itkToVtkConnector->SetInput(reader->GetOutput());
    try
    {
        itkToVtkConnector->Update();
    }
    catch (itk::ExceptionObject& e)
    {
        std::cerr << "exception in file reader" << std::endl;
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
    }

    /*actually, we can get vtkImageData from its output after transforming by itkToVtkImageFilter
     but we need to reslice it to 2d from 3d, 
    */  
    //vtkSmartPointer<vtkImageData> vtkImageData = itkToVtkConnector->GetOutput();

    auto reslicer = vtkSmartPointer<vtkImageReslice>::New();
    {
        reslicer->SetInputData(itkToVtkConnector->GetOutput());
        reslicer->SetInterpolationModeToCubic();//cubic spline interpolator
        reslicer->SetAutoCropOutput(true);
        reslicer->Update();
    }

    //get vtkImageData
    vtkSmartPointer<vtkImageData> m_imageData = vtkSmartPointer<vtkImageData>::New();
    m_imageData = reslicer->GetOutput();

    renderVtkImageData(m_imageData);

}

void renderVtkImageData(vtkSmartPointer<vtkImageData>& imageData)
{
    vtkSmartPointer<vtkImageViewer2> imageViewer =
        vtkSmartPointer<vtkImageViewer2>::New();

    imageViewer->SetInputData(imageData);

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    imageViewer->SetupInteractor(renderWindowInteractor);

    imageViewer->SetColorLevel(500);//设置窗位为500
    imageViewer->SetColorWindow(2000);//设置窗宽为2000
    imageViewer->SetSlice(40);//设置切片索引
    //imageViewer->SetSliceOrientationToXY();//设置切片方向
    imageViewer->Render();

    imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
    imageViewer->SetSize(640, 480);
    imageViewer->GetRenderWindow()->SetWindowName("DisplayImageExample");

    renderWindowInteractor->Start();
}