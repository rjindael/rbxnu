#include <App/V8/Tree/Instance.hpp>

namespace RNR
{
    World* Instance::world = 0;

    Instance::Instance()
    {
        m_node = 0;
        m_object = 0;
        m_parent = 0;
        setName("Instance");
    }

    Instance::~Instance()
    {
        setParent(NULL);
    }

    bool Instance::contains(Instance* child)
    {
        auto child_it = std::find(m_children.begin(), m_children.end(), child);            

        return child_it != m_children.end();
    }

    bool Instance::isAncestorOf(Instance* instance)
    {
        Instance* instance_parent = instance->m_parent;
        while (instance_parent != 0)
        {
            instance_parent = instance_parent->m_parent;

            if (instance_parent == this)
                return true;
        }

        return false;
    }

    bool Instance::askSetParent(Instance* instance)
    {
        return true;
    }

    bool Instance::canSetParent(Instance* instance)
    {
        return !instance || instance->canAddChild(this);
    }

    bool Instance::askAddChild(Instance* instance)
    {
        return true;
    }

    bool Instance::canAddChild(Instance* instance)
    {
        if (instance->contains(this) || instance->m_parent == this)
            return false;
        
        if (askAddChild(instance))
            return true;
        
        return instance->askSetParent(this);
    }

    void Instance::setName(std::string name)
    {
        if (name != this->m_name)
        {
            this->m_name = name;
            // raise property changed
        }
    }

    void Instance::setParent(Instance* newParent)
    {
        if (newParent != m_parent)
        {
            char error_text[255];
            if (this == newParent)
            {
                snprintf(error_text, 255, "Attempt to set %s as its own parent", m_name);
                throw std::runtime_error(error_text);
            }
            if (isAncestorOf(newParent))
            {
                snprintf(error_text, 255, "Attempt to set parent of %s to %s results in circular reference", newParent->getName(), m_name);
                throw std::runtime_error(error_text);
            }

            if (m_parent)
            {
                std::vector<Instance*>* children = m_parent->getChildren();
                auto child_it = std::find(children->begin(), children->end(), this);

                if (child_it != children->end())
                {
                    children->erase(child_it);
                    m_parent->onChildRemoved(this);
                
                    if (m_parent->numChildren() == 0)
                    {
                        // signal onlastchildremoved
                    }
                }
            }

            m_parent = newParent;
            if(m_parent)
            {
                m_parent->m_children.push_back(this);
                newParent->onChildAdded(this);
            }
        }
    }

    void Instance::onChildAdded(Instance* childAdded)
    {
        //
    }


    void Instance::onChildRemoved(RNR::Instance* childRemoved)
    {
        
    }
}