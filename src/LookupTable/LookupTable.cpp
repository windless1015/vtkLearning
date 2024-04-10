//#include <vtkActor.h>
//#include <vtkDataSetMapper.h>
//#include <vtkLookupTable.h>
//#include <vtkProperty.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkRenderer.h>
//#include <vtkSmartPointer.h>
//#include <vtkSphereSource.h>
//
//int main()
//{
//    // 创建Lookup Table
//    vtkSmartPointer<vtkLookupTable> lookupTable = vtkSmartPointer<vtkLookupTable>::New();
//    lookupTable->SetNumberOfTableValues(256);  // 定义256个颜色
//    lookupTable->Build();
//
//    // 填充Lookup Table
//    for (int i = 0; i < 256; i++) {
//        double r = static_cast<double>(i) / 255.0;
//        double g = 1.0 - r;
//        double b = 0.0;
//        lookupTable->SetTableValue(i, r, g, b, 1.0);  // 设置颜色和透明度
//    }
//
//    // 创建一个简单的球体数据源
//    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
//    sphereSource->Update();
//
//    // 映射器
//    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
//    mapper->SetInputConnection(sphereSource->GetOutputPort());
//    mapper->SetLookupTable(lookupTable);
//    mapper->SetScalarRange(0, 255);  // 设置标量值的范围，用于映射颜色
//
//    // Actor
//    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//    actor->SetMapper(mapper);
//
//    // 渲染器
//    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//    renderWindow->AddRenderer(renderer);
//    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//    renderWindowInteractor->SetRenderWindow(renderWindow);
//
//    // 添加Actor到渲染器
//    renderer->AddActor(actor);
//    renderer->SetBackground(0.1, 0.2, 0.4);  // 设置背景颜色
//
//    // 开始渲染过程
//    renderWindow->Render();
//    renderWindowInteractor->Start();
//
//    return EXIT_SUCCESS;
//}



#include <vtkSTLReader.h>
#include <vtkCamera.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapper3D.h>
#include <vtkLookupTable.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPCANormalEstimation.h>
#include <vtkPointSource.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkScalarBarActor.h>
#include <vtkSignedDistance.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkRegularPolygonSource.h>
#include <vtkPolyDataMapper.h>
// Readers
#include <vtkBYUReader.h>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkPolyDataReader.h>
#include <vtkSTLReader.h>
#include <vtkXMLPolyDataReader.h>

#include <vtkPointSource.h>
#include <vtkPolyData.h>

#include <algorithm> // For transform()
#include <cctype>    // For to_lower
#include <string>    // For find_last_of()

namespace {
    vtkSmartPointer<vtkPolyData> ReadPolyData(std::string const& fileName);
}

vtkSmartPointer<vtkImageActor> drawACircle();
int main(int argc, char* argv[])
{
    vtkNew<vtkNamedColors> colors;

    auto polyData = ReadPolyData(argc > 1 ? argv[1] : "");

    double bounds[6];
    polyData->GetBounds(bounds);
    double range[3];
    for (int i = 0; i < 3; ++i)
    {
        range[i] = bounds[2 * i + 1] - bounds[2 * i];
    }

    int sampleSize = polyData->GetNumberOfPoints() * .00005;
    if (sampleSize < 10)
    {
        sampleSize = 10;
    }
    std::cout << "Sample size is: " << sampleSize << std::endl;
    vtkNew<vtkPCANormalEstimation> normals;
    normals->SetInputData(polyData);
    normals->SetSampleSize(sampleSize);
    normals->SetNormalOrientationToGraphTraversal();
    normals->FlipNormalsOn();
    std::cout << "Range: " << range[0] << ", " << range[1] << ", " << range[2]
        << std::endl;
    int dimension = 256;
    dimension = 128;
    // auto radius = range[0] * 0.02;
    auto radius = range[0] / static_cast<double>(dimension) * 5; // ~5 voxels
    std::cout << "Radius: " << radius << std::endl;
    vtkNew<vtkSignedDistance> distance;
    distance->SetInputConnection(normals->GetOutputPort());
    distance->SetRadius(radius);
    distance->SetDimensions(dimension, dimension, dimension);
    distance->SetBounds(bounds[0] - range[0] * 0.1, bounds[1] + range[0] * 0.1,
        bounds[2] - range[1] * 0.1, bounds[3] + range[1] * 0.1,
        bounds[4] - range[2] * 0.1, bounds[5] + range[2] * 0.1);

    // Create a lookup table that consists of the full hue circle
    // (from HSV).
    auto belowRangeColor = colors->GetColor4d("Black").GetData();
    belowRangeColor[3] = 0.2;
    auto aboveRangeColor = colors->GetColor4d("White").GetData();
    aboveRangeColor[3] = 0.2;
    vtkNew<vtkLookupTable> hueLut;
    hueLut->SetTableRange(-0.99 * radius, 0.99 * radius);
    hueLut->SetHueRange(0.667, 0);
    hueLut->SetSaturationRange(1, 1);
    hueLut->SetValueRange(1, 1);
    hueLut->UseBelowRangeColorOn();
    hueLut->SetBelowRangeColor(belowRangeColor);
    hueLut->UseAboveRangeColorOn();
    hueLut->SetAboveRangeColor(aboveRangeColor);
    hueLut->SetNumberOfColors(5);
    hueLut->Build();
    double* last = hueLut->GetTableValue(4);
    hueLut->SetAboveRangeColor(last[0], last[1], last[2], 0);

    vtkNew<vtkImageMapToColors> sagittalColors;
    sagittalColors->SetInputConnection(distance->GetOutputPort());
    sagittalColors->SetLookupTable(hueLut);
    sagittalColors->Update();

   
    vtkSmartPointer<vtkImageActor> sagittal =  drawACircle();
    //vtkNew<vtkImageActor> sagittal;
    //sagittal->GetMapper()->SetInputConnection(sagittalColors->GetOutputPort());
    //sagittal->SetDisplayExtent(dimension / 2, dimension / 2, 0, dimension - 1, 0,
        //dimension - 1);
    //sagittal->ForceOpaqueOn();

    vtkNew<vtkImageMapToColors> axialColors;
    axialColors->SetInputConnection(distance->GetOutputPort());
    axialColors->SetLookupTable(hueLut);
    axialColors->Update();

    vtkNew<vtkImageActor> axial;
    axial->GetMapper()->SetInputConnection(axialColors->GetOutputPort());
    axial->SetDisplayExtent(0, dimension - 1, 0, dimension - 1, dimension / 2,
        dimension / 2);
    axial->ForceOpaqueOn();

    vtkNew<vtkImageMapToColors> coronalColors;
    coronalColors->SetInputConnection(distance->GetOutputPort());
    coronalColors->SetLookupTable(hueLut);
    coronalColors->Update();

    vtkNew<vtkImageActor> coronal;
    coronal->GetMapper()->SetInputConnection(coronalColors->GetOutputPort());
    coronal->SetDisplayExtent(0, dimension - 1, dimension / 2, dimension / 2, 0,
        dimension - 1);
    coronal->ForceOpaqueOn();

    // Create a scalar bar.
    vtkNew<vtkScalarBarActor> scalarBar;
    scalarBar->SetLookupTable(hueLut);
    scalarBar->SetTitle("Distance");
    scalarBar->SetNumberOfLabels(5);

    // Create graphics stuff.
    vtkNew<vtkRenderer> ren1;
    ren1->SetBackground(colors->GetColor3d("CornflowerBlue").GetData());

    vtkNew<vtkRenderWindow> renWin;
    renWin->AddRenderer(ren1);
    renWin->SetSize(600, 400);
    renWin->SetWindowName("SignedDistance");

    vtkNew<vtkRenderWindowInteractor> iren;
    iren->SetRenderWindow(renWin);

    vtkSmartPointer<vtkRegularPolygonSource> polygonSource = vtkSmartPointer<vtkRegularPolygonSource>::New();
    polygonSource->SetNumberOfSides(100);  // 使用较多的边数来近似一个圆形
    polygonSource->SetRadius(30);          // 设置圆的半径
    polygonSource->SetCenter(0.0, 0.0, 0.0); // 设置圆的中心位置
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(polygonSource->GetOutputPort());

    // 创建actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetOpacity(0.4); // 0.0 完全透明，1.0 完全不透明
    ren1->AddActor(actor);

    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName("D:\\DataRepo\\mesh\\upper.stl");
    reader->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper1->SetInputConnection(reader->GetOutputPort());
    vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
    actor1->SetMapper(mapper1);
    ren1->AddActor(actor1);

    // Add the actors to the renderer, set the background and size.
    //ren1->AddActor(sagittal);
    //ren1->AddActor(axial);
    //ren1->AddActor(coronal);
    //ren1->AddActor2D(scalarBar);

    // Generate an interesting view
    //
    ren1->ResetCamera();
    ren1->GetActiveCamera()->Azimuth(120);
    ren1->GetActiveCamera()->Elevation(30);
    ren1->GetActiveCamera()->Dolly(1.5);
    ren1->ResetCameraClippingRange();

    renWin->Render();
    iren->Initialize();
    iren->Start();
    std::cout << distance->GetOutput()->GetScalarRange()[0] << ", "
        << distance->GetOutput()->GetScalarRange()[1] << std::endl;

    return EXIT_SUCCESS;
}

namespace {
    vtkSmartPointer<vtkPolyData> ReadPolyData(std::string const& fileName)
    {
        vtkSmartPointer<vtkPolyData> polyData;
        std::string extension = "";
        if (fileName.find_last_of(".") != std::string::npos)
        {
            extension = fileName.substr(fileName.find_last_of("."));
        }
        // Make the extension lowercase.
        std::transform(extension.begin(), extension.end(), extension.begin(),
            ::tolower);
        if (extension == ".ply")
        {
            vtkNew<vtkPLYReader> reader;
            reader->SetFileName(fileName.c_str());
            reader->Update();
            polyData = reader->GetOutput();
        }
        else if (extension == ".vtp")
        {
            vtkNew<vtkXMLPolyDataReader> reader;
            reader->SetFileName(fileName.c_str());
            reader->Update();
            polyData = reader->GetOutput();
        }
        else if (extension == ".vtk")
        {
            vtkNew<vtkPolyDataReader> reader;
            reader->SetFileName(fileName.c_str());
            reader->Update();
            polyData = reader->GetOutput();
        }
        else if (extension == ".obj")
        {
            vtkNew<vtkOBJReader> reader;
            reader->SetFileName(fileName.c_str());
            reader->Update();
            polyData = reader->GetOutput();
        }
        else if (extension == ".stl")
        {
            vtkNew<vtkSTLReader> reader;
            reader->SetFileName(fileName.c_str());
            reader->Update();
            polyData = reader->GetOutput();
        }
        else if (extension == ".g")
        {
            vtkNew<vtkBYUReader> reader;
            reader->SetGeometryFileName(fileName.c_str());
            reader->Update();
            polyData = reader->GetOutput();
        }
        else
        {
            vtkNew<vtkMinimalStandardRandomSequence> randomSequence;
            randomSequence->SetSeed(8775070);

            vtkNew<vtkPointSource> points;
            points->SetNumberOfPoints(100000);
            points->SetRadius(10.0);
            double x, y, z;
            // random position
            x = randomSequence->GetRangeValue(-100, 100);
            randomSequence->Next();
            y = randomSequence->GetRangeValue(-100, 100);
            randomSequence->Next();
            z = randomSequence->GetRangeValue(-100, 100);
            randomSequence->Next();
            points->SetCenter(x, y, z);
            points->SetDistributionToShell();
            points->Update();
            polyData = points->GetOutput();
        }
        return polyData;
    }
} // namespace


vtkSmartPointer<vtkImageActor> drawACircle()
{
    // 创建一个图像画布，并设置其大小和背景颜色
    vtkSmartPointer<vtkImageCanvasSource2D> canvasSource = vtkSmartPointer<vtkImageCanvasSource2D>::New();
    canvasSource->SetExtent(0, 255, 0, 255, 0, 0);
    canvasSource->SetScalarTypeToUnsignedChar();
    canvasSource->SetNumberOfScalarComponents(4);  // 使用RGBA以支持透明度
    canvasSource->FillBox(0, 255, 0, 255);  // 填充背景

    // 设置透明背景
    canvasSource->SetDrawColor(0, 0, 0, 0); // 完全透明的黑色
    canvasSource->FillBox(0, 255, 0, 255);

    // 在透明背景上绘制一个不透明的白色圆
    canvasSource->SetDrawColor(255, 255, 255, 255); // 不透明的白色
    canvasSource->DrawCircle(128, 128, 80);  // 绘制圆

    canvasSource->Update();  // 更新画布以应用绘图操作

    // 创建一个图像actor
    vtkSmartPointer<vtkImageActor> imageActor = vtkSmartPointer<vtkImageActor>::New();
    imageActor->SetInputData(canvasSource->GetOutput());
    return imageActor;
}