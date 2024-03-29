#include "vtkipritem.h"

vtkCxxSetObjectMacro(vtkIPRItem, Graph, vtkGraph);
vtkStandardNewMacro(vtkIPRItem);



bool MyDrawing::Paint(vtkContext2D* painter)
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

bool MyDrawing::MouseButtonPressEvent(const vtkContextMouseEvent& mouse)
{
    this->MouseButtonPressed = mouse.GetButton();
    this->LastPosition[0] = mouse.GetPos()[0];
    this->LastPosition[1] = mouse.GetPos()[1];
    return true;
}







class vtkIPRItem::Implementation
{
public:
    Implementation()
    {
        Random = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
    }

    void CheckPositionSize(vtkIdType i)
    {
        while (i >= static_cast<vtkIdType>(this->Position.size()))
        {
            int size[2] = { 100, 100 };
            if (this->Item->GetScene())
            {
                this->Item->GetScene()->GetGeometry(size);
            }
            this->Random->Next();
            float x = static_cast<int>(this->Random->GetValue() * size[0]);
            this->Random->Next();
            float y = static_cast<int>(this->Random->GetValue() * size[1]);
            this->Position.push_back(std::make_pair(x, y));
        }
    }

    void GetPosition(vtkIdType i, float x[2])
    {
        this->CheckPositionSize(i);
        x[0] = this->Position[i].first;
        x[1] = this->Position[i].second;
    }

    void SetPosition(vtkIdType i, float x[2])
    {
        this->CheckPositionSize(i);
        this->Position[i] = std::make_pair(x[0], x[1]);
    }

    void CheckVelocitySize(vtkIdType i)
    {
        while (i >= static_cast<vtkIdType>(this->Velocity.size()))
        {
            this->Velocity.push_back(std::make_pair(0.0f, 0.0f));
        }
    }

    void GetVelocity(vtkIdType i, float x[2])
    {
        this->CheckVelocitySize(i);
        x[0] = this->Velocity[i].first;
        x[1] = this->Velocity[i].second;
    }

    void SetVelocity(vtkIdType i, float x[2])
    {
        this->CheckVelocitySize(i);
        this->Velocity[i] = std::make_pair(x[0], x[1]);
    }

    vtkSmartPointer<vtkMinimalStandardRandomSequence> Random;
    vtkIPRItem* Item;

    std::vector<std::pair<float, float> > Position;
    std::vector<std::pair<float, float> > Velocity;
};

//-----------------------------------------------------------------------------
vtkIPRItem::vtkIPRItem()
{
    this->Impl = new Implementation();
    this->Impl->Item = this;
    this->Graph = nullptr;
    this->MouseOver = false;
    this->MouseButtonPressed = -1;
    this->HitVertex = 0;
}

//-----------------------------------------------------------------------------
vtkIPRItem::~vtkIPRItem()
{
    delete this->Impl;
    this->SetGraph(nullptr);
}

//-----------------------------------------------------------------------------
bool vtkIPRItem::Paint(vtkContext2D* painter)
{
    painter->GetTextProp()->SetVerticalJustificationToCentered();
    painter->GetTextProp()->SetJustificationToCentered();
    painter->GetTextProp()->SetColor(0.0, 0.0, 0.0);
    painter->GetTextProp()->SetFontSize(12);
    painter->GetPen()->SetColorF(0.0f, 0.0f, 0.0f);
    painter->GetBrush()->SetColorF(0.8f, 0.8f, 1.0f, 0.5f);

    vtkSmartPointer<vtkEdgeListIterator> it = vtkSmartPointer<vtkEdgeListIterator>::New();
    this->Graph->GetEdges(it);
    float line[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    while (it->HasNext())
    {
        vtkEdgeType e = it->Next();
        this->Impl->GetPosition(e.Source, line);
        this->Impl->GetPosition(e.Target, line + 2);
        for (int i = 0; i < 4; ++i)
        {
            line[i] += 10.0f;
        }
        painter->DrawLine(line);
    }

    float dims[4] = { 0.0f, 0.0f, 20.0f, 20.0f };
    for (vtkIdType i = 0; i < this->Graph->GetNumberOfVertices(); ++i)
    {
        this->Impl->GetPosition(i, dims);
        painter->DrawRect(dims[0], dims[1], dims[2], dims[3]);
        float x = dims[0] + 0.5 * dims[2];
        float y = dims[1] + 0.5 * dims[3];
        painter->DrawString(x, y, vtkVariant(i).ToString());
    }

    return true;
}

//-----------------------------------------------------------------------------
bool vtkIPRItem::Hit(const vtkContextMouseEvent& mouse)
{
    float pos[2] = { 0.0f, 0.0f };
    for (vtkIdType i = this->Graph->GetNumberOfVertices() - 1; i >= 0; --i)
    {
        this->Impl->GetPosition(i, pos);
        if (mouse.GetPos()[0] > pos[0] &&
            mouse.GetPos()[0] < pos[0] + 20.0f &&
            mouse.GetPos()[1] > pos[1] &&
            mouse.GetPos()[1] < pos[1] + 20.0f)
        {
            this->HitVertex = i;
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
bool vtkIPRItem::MouseEnterEvent(const vtkContextMouseEvent&)
{
    this->MouseOver = true;
    return true;
}

//-----------------------------------------------------------------------------
bool vtkIPRItem::MouseMoveEvent(const vtkContextMouseEvent& mouse)
{
    int deltaX = static_cast<int>(mouse.GetPos()[0] - this->LastPosition[0]);
    int deltaY = static_cast<int>(mouse.GetPos()[1] - this->LastPosition[1]);
    this->LastPosition[0] = mouse.GetPos()[0];
    this->LastPosition[1] = mouse.GetPos()[1];

    if (this->MouseButtonPressed == 0)
    {
        // Move the vertex by this amount
        float pos[2];
        this->Impl->GetPosition(this->HitVertex, pos);
        pos[0] += deltaX;
        pos[1] += deltaY;
        this->Impl->SetPosition(this->HitVertex, pos);

        return true;
    }
#if 0
    if (this->MouseButtonPressed == 1)
    {
        if (deltaX > 0.0)
        {
            if (!this->GetTransform())
            {
                vtkSmartPointer<vtkTransform2D> t = vtkSmartPointer<vtkTransform2D>::New();
                t->Identity();
                this->SetTransform(t);
            }
            this->GetTransform()->Scale(deltaX / 50.0f, deltaX / 50.0f);
        }
        return true;
    }
#endif

    return false;
}

//-----------------------------------------------------------------------------
bool vtkIPRItem::MouseLeaveEvent(const vtkContextMouseEvent&)
{
    this->MouseOver = false;
    return true;
}

//-----------------------------------------------------------------------------
bool vtkIPRItem::MouseButtonPressEvent(const vtkContextMouseEvent& mouse)
{
    this->MouseButtonPressed = mouse.GetButton();
    this->LastPosition[0] = mouse.GetPos()[0];
    this->LastPosition[1] = mouse.GetPos()[1];
    return true;
}

//-----------------------------------------------------------------------------
bool vtkIPRItem::MouseButtonReleaseEvent(const vtkContextMouseEvent&)
{
    this->MouseButtonPressed = -1;
    return true;
}

//-----------------------------------------------------------------------------
void vtkIPRItem::UpdatePositions()
{
    vtkIdType numVerts = this->Graph->GetNumberOfVertices();
    float restDistance = 40.0f;
    float dampenLast = 0.5f;
    float springConstant = 0.3f;
    float repulseConstant = 1.0f;
    //float restDistance = 40.0f;
    //float dampenLast = 0.5f;
    //float springConstant = 0.1f;
    //float repulseConstant = 2.0f;
    float epsilon = 0.0000001f;
    float border = 20.0f;
    vtkSmartPointer<vtkOutEdgeIterator> it = vtkSmartPointer<vtkOutEdgeIterator>::New();
    float uPos[2];
    float vPos[2];
    float uVel[2];
    int geom[2] = { 100, 100 };
    if (this->GetScene())
    {
        this->GetScene()->GetGeometry(geom);
    }
    for (vtkIdType u = 0; u < numVerts; ++u)
    {
        if (this->MouseButtonPressed == 0 && u == this->HitVertex)
        {
            continue;
        }
        this->Impl->GetPosition(u, uPos);
        float fx = 0.0;
        float fy = 0.0;
        for (vtkIdType v = 0; v < numVerts; ++v)
        {
            this->Impl->GetPosition(v, vPos);
            float deltaX = uPos[0] - vPos[0];
            float deltaY = uPos[1] - vPos[1];
            float distSquared = deltaX * deltaX + deltaY * deltaY;
            // Avoid divide by zero
            distSquared += epsilon;
            fx += repulseConstant * deltaX / distSquared;
            fy += repulseConstant * deltaY / distSquared;
        }
        this->Graph->GetOutEdges(u, it);
        while (it->HasNext())
        {
            vtkOutEdgeType e = it->Next();
            vtkIdType v = e.Target;
            if (u == v)
            {
                continue;
            }
            this->Impl->GetPosition(v, vPos);
            float deltaX = uPos[0] - vPos[0];
            float deltaY = uPos[1] - vPos[1];
            float dist = sqrt(deltaX * deltaX + deltaY * deltaY);
            float force = springConstant * (dist - restDistance);
            fx -= force * deltaX / dist;
            fy -= force * deltaY / dist;
        }
        float center[2] = { uPos[0] + 10.0f, uPos[1] + 10.0f };
        // Change the force if it is near the edge
        if (center[0] < border)
        {
            fx += -(center[0] - border);
        }
        else if (center[0] > geom[0] - border)
        {
            fx += -(center[0] - (geom[0] - border));
        }
        if (center[1] < border)
        {
            fy += -(center[1] - border);
        }
        else if (center[1] > geom[1] - border)
        {
            fy += -(center[1] - (geom[1] - border));
        }
        // Update velocity and position
        this->Impl->GetVelocity(u, uVel);
        uVel[0] = dampenLast * uVel[0] + fx;
        uVel[1] = dampenLast * uVel[1] + fy;
        uPos[0] += uVel[0];
        uPos[1] += uVel[1];
        this->Impl->SetPosition(u, uPos);
        this->Impl->SetVelocity(u, uVel);
    }
}

//-----------------------------------------------------------------------------
void vtkIPRItem::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}
