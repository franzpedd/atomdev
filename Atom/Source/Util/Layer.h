#pragma once
#include "Core/Core.h"

#include <vector>
#include <algorithm>

namespace Atom
{
    class API Layer
    {
    public:

        // constructor
        Layer(const char* name = "Layer") : m_Name(name) {}

        // destructor
        virtual ~Layer() = default;

        // layer loop
        virtual void Update() {};

        // returns layer's name
        inline const char* GetName() { return m_Name; }

    private:
        
        const char* m_Name;
    };

    class API LayerStack
    {
    public:

        // constructor
        LayerStack() = default;

        // destructor
        ~LayerStack()
        {
            for(Layer* layer : m_Layers)
            {
                delete layer;
            }
        }

    public:

        // pushes to stack's insertion index
        void Push(Layer* layer)
        {
            m_Layers.emplace(m_Layers.begin() + m_Index, layer);
            m_Index++;
        }

        // pops from stack's insertion index
        void Pop(Layer* layer)
        {
            auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_Index, layer);
            if(it != m_Layers.begin() + m_Index)
            {
                m_Layers.erase(it);
                m_Index--;
            }
        }

        // pushes over to the back
        void PushOver(Layer* layer)
        {
            m_Layers.emplace_back(layer);
        }

        // pops from over the back-half
        void PopOver(Layer* layer)
        {
            auto it = std::find(m_Layers.begin() + m_Index, m_Layers.end(), layer);
            if(it != m_Layers.end())
            {
                m_Layers.erase(it);
            }
        }

    public:

        // iterator for ranged loop
        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
        std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

        // const iterators for const ranged loops
        std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
        std::vector<Layer*>::const_iterator end() const { return m_Layers.end(); }
        std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
        std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

    private:
        
        std::vector<Layer*> m_Layers;
        unsigned int m_Index = 0;
    };
}