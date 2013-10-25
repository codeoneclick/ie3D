//
//  CGUIGraph.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 10/10/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGUIGraph_h
#define CGUIGraph_h

#include "HCommon.h"
#include "IGraph.h"

class IGUIControl;
class CRenderMgr;
class CSceneUpdateMgr;
class CGUIContainer;
class CGUIButton;

class CGUIGraph : public virtual IGraph
{
private:

protected:

	std::set<std::shared_ptr<IGUIControl> > m_controlsContainer;
    
    void _InsertGUIControl(const std::shared_ptr<IGUIControl>& _control);
    void _RemoveGUIControl(const std::shared_ptr<IGUIControl>& _control);

public:

	CGUIGraph(void);
	~CGUIGraph(void);

	void InsertGUIContainer(const std::shared_ptr<CGUIContainer>& _container);
    void RemoveGUIContainer(const std::shared_ptr<CGUIContainer>& _container);
    
    void InsertGUIButton(const std::shared_ptr<CGUIButton>& _button);
    void RemoveGUIButton(const std::shared_ptr<CGUIButton>& _button);
};

#endif 
