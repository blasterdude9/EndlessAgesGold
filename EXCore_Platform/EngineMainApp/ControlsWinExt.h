#include "axlanguagefilter.h"
#include "listTestClass.h"
#include "ItemManagerDlg.h"
#include "ServerCurrency.h"

#ifndef _ClientOutput
struct EditorUndoEntry
{
	int       traceId;
	int       groupIndex;
	int       libReference;
	D3DMATRIX position;
	BOOL      hasNodeInfo;
};

static EditorUndoEntry g_editorUndoStack[512];
static int             g_editorUndoCount = 0;
static EditorUndoEntry g_editorRedoStack[512];
static int             g_editorRedoCount = 0;
static DWORD           g_editorLastUndoTick = 0;

static void PushEditorUndoObjectTrace(int traceId, int groupIndex = 0, int libRef = -1, const D3DMATRIX * pos = NULL)
{
	if(g_editorUndoCount >= 512)
	{
		memmove(&g_editorUndoStack[0], &g_editorUndoStack[1], sizeof(EditorUndoEntry) * 511);
		g_editorUndoCount = 511;
	}
	EditorUndoEntry & e = g_editorUndoStack[g_editorUndoCount++];
	e.traceId      = traceId;
	e.groupIndex   = groupIndex;
	e.libReference = libRef;
	if(pos)
	{
		e.position    = *pos;
		e.hasNodeInfo = (libRef >= 0) ? TRUE : FALSE;
	}
	else
	{
		memset(&e.position, 0, sizeof(D3DMATRIX));
		e.hasNodeInfo = FALSE;
	}
}
#endif

static int g_editorHighlightTraceId     = -1;
static int g_editorHighlightGroupId     = -99999;
static int g_editorHighlightEntityIndex = -1;
static int g_editorLast3DPickedObjIndex = -1;

static void AddEditorBoxLines(CDrawVectors & dVec, const D3DVECTOR & minPt, const D3DVECTOR & maxPt, bool withDiagonals)
{
	D3DVECTOR c000 = { minPt.x, minPt.y, minPt.z };
	D3DVECTOR c100 = { maxPt.x, minPt.y, minPt.z };
	D3DVECTOR c101 = { maxPt.x, minPt.y, maxPt.z };
	D3DVECTOR c001 = { minPt.x, minPt.y, maxPt.z };

	D3DVECTOR c010 = { minPt.x, maxPt.y, minPt.z };
	D3DVECTOR c110 = { maxPt.x, maxPt.y, minPt.z };
	D3DVECTOR c111 = { maxPt.x, maxPt.y, maxPt.z };
	D3DVECTOR c011 = { minPt.x, maxPt.y, maxPt.z };

	// Bottom rectangle
	dVec.AddLineWLD(c000, c100);
	dVec.AddLineWLD(c100, c101);
	dVec.AddLineWLD(c101, c001);
	dVec.AddLineWLD(c001, c000);

	// Top rectangle
	dVec.AddLineWLD(c010, c110);
	dVec.AddLineWLD(c110, c111);
	dVec.AddLineWLD(c111, c011);
	dVec.AddLineWLD(c011, c010);

	// Vertical pillars
	dVec.AddLineWLD(c000, c010);
	dVec.AddLineWLD(c100, c110);
	dVec.AddLineWLD(c101, c111);
	dVec.AddLineWLD(c001, c011);

	if(withDiagonals)
	{
		// 3D X-cross diagonals through center
		dVec.AddLineWLD(c000, c111);
		dVec.AddLineWLD(c100, c011);
		dVec.AddLineWLD(c001, c110);
		dVec.AddLineWLD(c101, c010);

		// Vertical beacon above top center
		D3DVECTOR topCenter = { (minPt.x + maxPt.x) * 0.5f, maxPt.y, (minPt.z + maxPt.z) * 0.5f };
		D3DVECTOR beaconTop = { topCenter.x, maxPt.y + 12.0f, topCenter.z };
		dVec.AddLineWLD(topCenter, beaconTop);
	}
}

static bool GetEditorWorldObjectBounds(CWldObject * ptrLoc, D3DVECTOR & minPt, D3DVECTOR & maxPt, D3DVECTOR & centerPt)
{
	if(!ptrLoc)
		return false;

	if(ptrLoc->m_meshObject)
	{
		ABBOX & bb = ptrLoc->m_meshObject->List_boundingBox;
		if(bb.minX == 0.0f && bb.maxX == 0.0f &&
		   bb.minY == 0.0f && bb.maxY == 0.0f &&
		   bb.minZ == 0.0f && bb.maxZ == 0.0f)
		{
			ptrLoc->m_meshObject->GetBox();
		}
		minPt.x = bb.minX; minPt.y = bb.minY; minPt.z = bb.minZ;
		maxPt.x = bb.maxX; maxPt.y = bb.maxY; maxPt.z = bb.maxZ;
	}
	else if(ptrLoc->m_node)
	{
		ABBOX & cb = ptrLoc->m_node->m_clipBox;
		if(cb.minX != 0.0f || cb.maxX != 0.0f ||
		   cb.minY != 0.0f || cb.maxY != 0.0f ||
		   cb.minZ != 0.0f || cb.maxZ != 0.0f)
		{
			minPt.x = cb.minX; minPt.y = cb.minY; minPt.z = cb.minZ;
			maxPt.x = cb.maxX; maxPt.y = cb.maxY; maxPt.z = cb.maxZ;
		}
		else
		{
			D3DVECTOR p = { ptrLoc->m_node->m_position._41, ptrLoc->m_node->m_position._42, ptrLoc->m_node->m_position._43 };
			minPt.x = p.x - 1.5f; minPt.y = p.y - 1.5f; minPt.z = p.z - 1.5f;
			maxPt.x = p.x + 1.5f; maxPt.y = p.y + 1.5f; maxPt.z = p.z + 1.5f;
		}
	}
	else if(ptrLoc->m_lightObject)
	{
		D3DVECTOR p = ptrLoc->m_lightObject->m_wldspacePos;
		minPt.x = p.x - 1.0f; minPt.y = p.y - 1.0f; minPt.z = p.z - 1.0f;
		maxPt.x = p.x + 1.0f; maxPt.y = p.y + 1.0f; maxPt.z = p.z + 1.0f;
	}
	else
	{
		D3DVECTOR p;
		p.x = (ptrLoc->List_popoutBox.min.x + ptrLoc->List_popoutBox.max.x) * 0.5f;
		p.y = (ptrLoc->List_popoutBox.min.y + ptrLoc->List_popoutBox.max.y) * 0.5f;
		p.z = (ptrLoc->List_popoutBox.min.z + ptrLoc->List_popoutBox.max.z) * 0.5f;
		minPt.x = p.x - 1.5f; minPt.y = p.y - 1.5f; minPt.z = p.z - 1.5f;
		maxPt.x = p.x + 1.5f; maxPt.y = p.y + 1.5f; maxPt.z = p.z + 1.5f;
	}

	if(minPt.x > maxPt.x) { float t = minPt.x; minPt.x = maxPt.x; maxPt.x = t; }
	if(minPt.y > maxPt.y) { float t = minPt.y; minPt.y = maxPt.y; maxPt.y = t; }
	if(minPt.z > maxPt.z) { float t = minPt.z; minPt.z = maxPt.z; maxPt.z = t; }

	centerPt.x = (minPt.x + maxPt.x) * 0.5f;
	centerPt.y = (minPt.y + maxPt.y) * 0.5f;
	centerPt.z = (minPt.z + maxPt.z) * 0.5f;
	return true;
}

static void PadEditorHighlightBox(const D3DVECTOR & minPt, const D3DVECTOR & maxPt, const D3DVECTOR & centerPt, D3DVECTOR & drawMin, D3DVECTOR & drawMax)
{
	drawMin.x = minPt.x - 0.15f; drawMax.x = maxPt.x + 0.15f;
	drawMin.y = minPt.y - 0.15f; drawMax.y = maxPt.y + 0.15f;
	drawMin.z = minPt.z - 0.15f; drawMax.z = maxPt.z + 0.15f;
	if(drawMax.x - drawMin.x < 1.0f) { drawMin.x = centerPt.x - 0.5f; drawMax.x = centerPt.x + 0.5f; }
	if(drawMax.y - drawMin.y < 1.0f) { drawMin.y = centerPt.y - 0.5f; drawMax.y = centerPt.y + 0.5f; }
	if(drawMax.z - drawMin.z < 1.0f) { drawMin.z = centerPt.z - 0.5f; drawMax.z = centerPt.z + 0.5f; }
}

static void UpdateEditorObjectHighlight(GlobalStatusVariables * gStatus, BOOL objectBarVisible, BOOL entityBarVisible)
{
	if(!gStatus)
		return;

	if(objectBarVisible && gStatus->m_wldObjList)
	{
		if(g_editorHighlightTraceId != -1)
		{
			for(POSITION posLoc = gStatus->m_wldObjList->GetHeadPosition(); posLoc != NULL;)
			{
				CWldObject * ptrLoc = (CWldObject *)gStatus->m_wldObjList->GetNext(posLoc);
				if(ptrLoc && ptrLoc->List_identity == g_editorHighlightTraceId)
				{
					D3DVECTOR minPt, maxPt, centerPt, drawMin, drawMax;
					if(GetEditorWorldObjectBounds(ptrLoc, minPt, maxPt, centerPt))
					{
						PadEditorHighlightBox(minPt, maxPt, centerPt, drawMin, drawMax);
						gStatus->m_dVectors.Clear();
						AddEditorBoxLines(gStatus->m_dVectors, drawMin, drawMax, true);

						if(gStatus->m_entObjRuntime)
						{
							POSITION ePos = gStatus->m_entObjRuntime->FindIndex(0);
							if(ePos)
							{
								CMovementObj * entPtr = (CMovementObj *)gStatus->m_entObjRuntime->GetAt(ePos);
								if(entPtr && entPtr->List_baseFrame)
								{
									D3DVECTOR playerPos;
									entPtr->List_baseFrame->GetPosition(NULL, &playerPos);
									playerPos.y += 1.5f;
									gStatus->m_dVectors.AddLineWLD(playerPos, centerPt);
								}
							}
						}
					}
					return;
				}
			}
		}
		else if(g_editorHighlightGroupId != -99999)
		{
			gStatus->m_dVectors.Clear();
			for(POSITION posLoc = gStatus->m_wldObjList->GetHeadPosition(); posLoc != NULL;)
			{
				CWldObject * ptrLoc = (CWldObject *)gStatus->m_wldObjList->GetNext(posLoc);
				if(!ptrLoc) continue;
				BOOL match = FALSE;
				if(g_editorHighlightGroupId == -9998)
				{
					if(ptrLoc->m_filterGroup != -1) match = TRUE;
				}
				else if(g_editorHighlightGroupId == -9999)
				{
					if(ptrLoc->m_particleLink != -1 || ptrLoc->m_filterGroup != -1 ||
					   ptrLoc->List_usedAsSpawnPoint != -1 || ptrLoc->m_lightObject ||
					   ptrLoc->List_removeEnviorment ||
					   (ptrLoc->List_triggerList && ptrLoc->List_triggerList->GetCount() > 0) ||
					   (ptrLoc->m_meshObject && ptrLoc->m_meshObject->m_charona))
						match = TRUE;
				}
				else if(ptrLoc->List_groupNumber == g_editorHighlightGroupId)
				{
					match = TRUE;
				}
				if(match)
				{
					D3DVECTOR minPt, maxPt, centerPt, drawMin, drawMax;
					if(GetEditorWorldObjectBounds(ptrLoc, minPt, maxPt, centerPt))
					{
						PadEditorHighlightBox(minPt, maxPt, centerPt, drawMin, drawMax);
						AddEditorBoxLines(gStatus->m_dVectors, drawMin, drawMax, false);
					}
				}
			}
			return;
		}
	}
	else if(entityBarVisible && g_editorHighlightEntityIndex != -1 && gStatus->m_entObjRuntime)
	{
		POSITION posLoc = gStatus->m_entObjRuntime->FindIndex(g_editorHighlightEntityIndex);
		if(posLoc)
		{
			CMovementObj * ptrLoc = (CMovementObj *)gStatus->m_entObjRuntime->GetAt(posLoc);
			if(ptrLoc && ptrLoc->List_baseFrame)
			{
				D3DVECTOR entPos;
				ptrLoc->List_baseFrame->GetPosition(NULL, &entPos);
				D3DVECTOR drawMin = { entPos.x - 1.2f, entPos.y - 2.0f, entPos.z - 1.2f };
				D3DVECTOR drawMax = { entPos.x + 1.2f, entPos.y + 3.0f, entPos.z + 1.2f };
				gStatus->m_dVectors.Clear();
				AddEditorBoxLines(gStatus->m_dVectors, drawMin, drawMax, true);

				POSITION ePos0 = gStatus->m_entObjRuntime->FindIndex(0);
				if(ePos0 && g_editorHighlightEntityIndex != 0)
				{
					CMovementObj * playerPtr = (CMovementObj *)gStatus->m_entObjRuntime->GetAt(ePos0);
					if(playerPtr && playerPtr->List_baseFrame)
					{
						D3DVECTOR playerPos;
						playerPtr->List_baseFrame->GetPosition(NULL, &playerPos);
						playerPos.y += 1.5f;
						gStatus->m_dVectors.AddLineWLD(playerPos, entPos);
					}
				}
			}
		}
	}
}

static void TeleportEditorToSelectedObject(GlobalStatusVariables * gStatus)
{
	if(!gStatus || !gStatus->m_wldObjList || !gStatus->m_entObjRuntime || g_editorHighlightTraceId == -1)
		return;

	for(POSITION posLoc = gStatus->m_wldObjList->GetHeadPosition(); posLoc != NULL;)
	{
		CWldObject * ptrLoc = (CWldObject *)gStatus->m_wldObjList->GetNext(posLoc);
		if(ptrLoc && ptrLoc->List_identity == g_editorHighlightTraceId)
		{
			D3DVECTOR minPt, maxPt, centerPt;
			if(!GetEditorWorldObjectBounds(ptrLoc, minPt, maxPt, centerPt))
				return;

			POSITION ePos = gStatus->m_entObjRuntime->FindIndex(0);
			if(!ePos)
				return;
			CMovementObj * entPtr = (CMovementObj *)gStatus->m_entObjRuntime->GetAt(ePos);
			if(!entPtr || !entPtr->List_baseFrame)
				return;

			D3DVECTOR curPos;
			entPtr->List_baseFrame->GetPosition(NULL, &curPos);
			float dx = curPos.x - centerPt.x;
			float dz = curPos.z - centerPt.z;
			float horizLen = (float)sqrt(dx * dx + dz * dz);
			float halfSpan = (float)max(maxPt.x - minPt.x, maxPt.z - minPt.z) * 0.5f;
			float standDist = halfSpan + 6.0f;
			if(standDist > 40.0f) standDist = 12.0f;

			D3DVECTOR targetPos;
			if(horizLen > 0.01f)
			{
				targetPos.x = centerPt.x + (dx / horizLen) * standDist;
				targetPos.z = centerPt.z + (dz / horizLen) * standDist;
			}
			else
			{
				targetPos.x = centerPt.x;
				targetPos.z = centerPt.z - standDist;
			}
			targetPos.y = centerPt.y + 2.0f;

			entPtr->List_baseFrame->SetPosition2(NULL, targetPos.x, targetPos.y, targetPos.z);
			if(entPtr->List_movementDB)
				entPtr->List_baseFrame->GetPosition(NULL, &entPtr->List_movementDB->List_lastPosition);
			if(entPtr->m_collisionRaySys)
				entPtr->m_collisionRaySys->InitOnSpawn(entPtr->List_baseFrame, entPtr->List_skeletalObject, &gStatus->m_matLib);

			if(gStatus->m_systemReadout)
			{
				CString msg;
				msg.Format("Teleported to TRC_%d at (%.1f, %.1f, %.1f)", ptrLoc->List_identity, centerPt.x, centerPt.y, centerPt.z);
				gStatus->m_systemReadout->AddLine(msg);
			}
			return;
		}
	}
}

void EngineXWin::ControlsInLoop()
{
	if(!m_userControls)
		return;

	if((m_userControls->m_input->m_keyboardKeys[DIK_LCONTROL].IsDown() ||
	    m_userControls->m_input->m_keyboardKeys[DIK_RCONTROL].IsDown()) &&
	   m_userControls->m_input->m_keyboardKeys[DIK_Z].IsDown())
	{
		#ifndef _ClientOutput
		DWORD nowTick = GetTickCount();
		if(nowTick - g_editorLastUndoTick >= 250)
		{
			g_editorLastUndoTick = nowTick;
			if(g_editorUndoCount > 0)
			{
				EditorUndoEntry entry = g_editorUndoStack[--g_editorUndoCount];
				if(!entry.hasNodeInfo && m_gStatus && m_gStatus->m_wldObjList)
				{
					for(POSITION p = m_gStatus->m_wldObjList->GetHeadPosition(); p != NULL;)
					{
						CWldObject * wObj = (CWldObject*)m_gStatus->m_wldObjList->GetNext(p);
						if(wObj && wObj->List_identity == entry.traceId && wObj->m_node)
						{
							entry.groupIndex   = wObj->List_groupNumber;
							entry.libReference = wObj->m_node->m_libraryReference;
							entry.position     = wObj->m_node->m_position;
							entry.hasNodeInfo  = TRUE;
							break;
						}
					}
				}
				if(entry.hasNodeInfo && g_editorRedoCount < 512)
					g_editorRedoStack[g_editorRedoCount++] = entry;

				DeleteObjectByTrace(entry.traceId);
				m_gStatus->m_resetReferences = TRUE;
				m_gStatus->m_skeletalReferenceObjCache.RemoveAll();
				if(m_gStatus->m_systemReadout)
					m_gStatus->m_systemReadout->AddLine(operator+("Undo: Removed object TRC_", m_gStatus->m_cabLib.ConvToString(entry.traceId)));
			}
			else if(m_gStatus->m_systemReadout)
			{
				m_gStatus->m_systemReadout->AddLine("Undo: Nothing to undo");
			}
		}
		return;
		#endif
	}

	if((m_userControls->m_input->m_keyboardKeys[DIK_LCONTROL].IsDown() ||
	    m_userControls->m_input->m_keyboardKeys[DIK_RCONTROL].IsDown()) &&
	   m_userControls->m_input->m_keyboardKeys[DIK_Y].IsDown())
	{
		#ifndef _ClientOutput
		DWORD nowTick = GetTickCount();
		if(nowTick - g_editorLastUndoTick >= 250)
		{
			g_editorLastUndoTick = nowTick;
			if(g_editorRedoCount > 0)
			{
				EditorUndoEntry entry = g_editorRedoStack[--g_editorRedoCount];
				AddNodeByData(entry.groupIndex, entry.libReference, NULL, &entry.position);
				m_gStatus->m_resetReferences = TRUE;
				m_gStatus->m_skeletalReferenceObjCache.RemoveAll();
				if(m_gStatus->m_systemReadout)
					m_gStatus->m_systemReadout->AddLine("Redo: Restored placed object");
			}
			else if(m_gStatus->m_systemReadout)
			{
				m_gStatus->m_systemReadout->AddLine("Redo: Nothing to redo");
			}
		}
		return;
		#endif
	}

	if((m_userControls->m_input->m_keyboardKeys[DIK_LCONTROL].IsDown() ||
	    m_userControls->m_input->m_keyboardKeys[DIK_RCONTROL].IsDown()) &&
	   m_userControls->m_input->m_keyboardKeys[DIK_D].IsDown())
	{
		#ifndef _ClientOutput
		DWORD nowTick = GetTickCount();
		if(nowTick - g_editorLastUndoTick >= 250)
		{
			g_editorLastUndoTick = nowTick;
			CWldObject * srcObj = NULL;
			HWND treeHwnd = NULL;
			m_dlgBarObject.GetDlgItem(IDC_OBJECT_TREE, &treeHwnd);
			if(treeHwnd)
			{
				HTREEITEM item = TreeView_GetSelection(treeHwnd);
				if(item)
				{
					CString textLoc = GetTreeItemText(IDC_OBJECT_TREE, &m_dlgBarObject, item, 10);
					for(POSITION p = m_gStatus->m_wldObjList->GetHeadPosition(); p != NULL;)
					{
						CWldObject * wObj = (CWldObject*)m_gStatus->m_wldObjList->GetNext(p);
						if(wObj && operator+("TRC_", m_gStatus->m_cabLib.ConvToString(wObj->List_identity)) == textLoc)
						{
							srcObj = wObj;
							break;
						}
					}
				}
			}
			if(!srcObj && g_editorUndoCount > 0)
			{
				int lastTrace = g_editorUndoStack[g_editorUndoCount - 1].traceId;
				for(POSITION p = m_gStatus->m_wldObjList->GetHeadPosition(); p != NULL;)
				{
					CWldObject * wObj = (CWldObject*)m_gStatus->m_wldObjList->GetNext(p);
					if(wObj && wObj->List_identity == lastTrace)
					{
						srcObj = wObj;
						break;
					}
				}
			}
			if(srcObj && srcObj->m_node)
			{
				D3DMATRIX dupMat = srcObj->m_node->m_position;
				AddNodeByData(srcObj->List_groupNumber, srcObj->m_node->m_libraryReference, NULL, &dupMat);
				m_gStatus->m_resetReferences = TRUE;
				m_gStatus->m_skeletalReferenceObjCache.RemoveAll();
				if(m_gStatus->m_systemReadout)
					m_gStatus->m_systemReadout->AddLine(operator+("Duplicated object TRC_", m_gStatus->m_cabLib.ConvToString(srcObj->List_identity)));
			}
		}
		return;
		#endif
	}

	if((m_userControls->m_input->m_keyboardKeys[DIK_LCONTROL].IsDown() ||
	    m_userControls->m_input->m_keyboardKeys[DIK_RCONTROL].IsDown()) &&
	   m_userControls->m_input->m_keyboardKeys[DIK_F].IsDown())
	{
		#ifndef _ClientOutput
		DWORD nowTick = GetTickCount();
		if(nowTick - g_editorLastUndoTick >= 250)
		{
			g_editorLastUndoTick = nowTick;
			TeleportEditorToSelectedObject(m_gStatus);
		}
		return;
		#endif
	}

	if(m_userControls->m_input->m_keyboardKeys[DIK_RCONTROL].IsDown() && m_userControls->m_input->m_keyboardKeys[DIK_Q].IsDown())
	{
		m_gStatus->SetCloseDownCallState(TRUE);
		return;
	}

	if(m_userControls->m_input->m_keyboardKeys[DIK_LCONTROL].IsDown() && m_userControls->m_input->m_keyboardKeys[DIK_Q].IsDown())
	{
		m_gStatus->SetCloseDownCallState(TRUE);
		return;
	}

	if(m_userControls->m_input->m_keyboardKeys[DIK_LCONTROL].IsDown() && m_userControls->m_input->m_keyboardKeys[DIK_F10].IsDown())
	{
		CItemManagerDlg dlg;
		dlg.globalStatusMem = m_gStatus;
		dlg.DoModal();
		return;
	}

	if(m_userControls->m_input->m_keyboardKeys[DIK_F5].IsDown())
	{
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		long currentTime = GetTickCount();
		if(!entPosition)
			return;

		CMovementObj * entityPtr = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
		entityPtr->m_attackAnimOverride = 1;

		CBoneAnimationObject* animPtrRet = NULL;
		entityPtr->List_skeletalObject->m_secondaryAnimationInUse = entityPtr->List_skeletalObject->GetAnimationBasedOnAttribAndPref(12,entityPtr->m_attackAnimOverride,&animPtrRet);

		entityPtr->SetSecondaryTimingIntro(entityPtr->List_skeletalObject,currentTime);
		if(animPtrRet)
			entityPtr->List_skeletalObject->m_scndDurationCurrentLength = animPtrRet->m_minPlayDuration;
		entityPtr->List_skeletalObject->m_lastScndDurationUpdateTime = currentTime;
	}

	if(m_userControls->m_input->m_keyboardKeys[DIK_F6].IsDown())
	{
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(!entPosition)
			return;

		CMovementObj * entityPtr = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
		CArmedInventoryObj *visObj = entityPtr->List_armedInventoryDB->GetByType(92);
		CFireRangeObject *fireObj = NULL;
		int i = 0;
		if(visObj)
		{
			if(visObj->m_firstPersonSystem)
				if(visObj->m_firstPersonSystem->m_fireQueList)
					fireObj = visObj->m_firstPersonSystem->m_fireQueList->GetByFireQueIndex(i);
			if(visObj->m_thirdPersonSystem)
				if(visObj->m_thirdPersonSystem->m_fireQueList)
					fireObj = visObj->m_thirdPersonSystem->m_fireQueList->GetByFireQueIndex(i);
		}

		if(fireObj)
			fireObj->m_missileIndex = 6;

		entityPtr->ChangeMissileIndexByLoc(0, 12, 13);
	}
 
	if(m_userControls->m_input->m_keyboardKeys[DIK_TAB].IsDown())
	{
		#ifdef _chinaClient
		if((GetTickCount()-m_gStatus->m_lastGlobalUnicodeIndexUse) > 300)
		{
			for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();posLoc != NULL;)
			{//Begin movement entity loop
				CMovementObj * ptrLoc = (CMovementObj *) m_gStatus->m_entObjRuntime->GetNext(posLoc);		    	
				if(ptrLoc->List_controlType != -1)
				{//if has a controller attached
					if(ptrLoc->List_controlType == 0)
					{//control config in use
						if(m_gStatus->m_speechMode==0)
						{
							m_gStatus->m_speechMode = 1;
							int strLen = ptrLoc->List_networkTextString.GetLength();
							if(strLen > 0)
							{
								if(ptrLoc->List_networkTextString.GetAt(strLen-1)!='~')
									ptrLoc->List_networkTextString = ptrLoc->List_networkTextString + "~";
							}
						}
						else
						{
							m_gStatus->m_speechMode = 0;
							int strLen = ptrLoc->List_networkTextString.GetLength();
							if(strLen > 0)
							{
								if(ptrLoc->List_networkTextString.GetAt(strLen-1)=='~')
									ptrLoc->List_networkTextString.Delete(strLen-1,1);
							}
						}
						m_gStatus->m_lastGlobalUnicodeIndexUse = GetTickCount();
						return;
					}
				}
			}
		}
		return;
		#endif

		CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_LOOT_INVENTORY, true));
		if(!pInventory)
			return;
		// CLEAR the inventory list of old items
		pInventory->ClearItems();

/*		int iCount = 0;
		for (int i=0; i<1; i++)
		{
			// PARSE out the next item struct
			ITEMDATA_STRUCT_EX item;
			//CopyMemory(&item, &pItemData[iCount], sizeof(item));
			//iCount += sizeof(item);
			item.m_armed = false;
			item.m_dMod = 0;
			item.m_quantity = 1;
			item.m_received = 1;
			item.m_GLID = 351;//concussion blade

			// GET the item by its GLID
			CItemObj *pItem = m_gStatus->m_itemManager->GetItemByID(item.m_GLID);
			if (!pItem) continue;

			// ADD the loot item to the inventory
			pInventory->AddItem(item);
		}
*/


		Sleep(1000);
	}

	if(m_userControls->m_input->m_keyboardKeys[DIK_ESCAPE].IsDown())
	{
		#ifndef _ClientOutput
		if(m_gStatus->m_dxRenderer->m_fullScreenMode == TRUE)
		{
			m_gStatus->CreateDisplayIM(FALSE,0,0,640,480,16);
			return;
		}
		if(m_gStatus->m_dxRenderer->m_showMouse == FALSE)
		{
			m_gStatus->m_dxRenderer->EnableMouse();
			return;
		}
		#endif
	}

	if(m_userControls->m_input->m_keyboardKeys[DIK_F8].IsDown())
	{  
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(!entPosition)
			return;

		CMovementObj * entityPtr = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);

		if(GetTickCount()-CountGlob < 100)
			return;

		CountGlob = GetTickCount();

		D3DVECTOR expPoint,forceVect;

		ZeroMemory(&expPoint,sizeof(D3DVECTOR));

		entityPtr->ApplyForce(expPoint,1.0f,50.0f);
	}

	if(m_userControls->m_input->m_keyboardKeys[DIK_F7].IsDown())
	{  
        /*CServerCurrencySystem * cSys = new CServerCurrencySystem();
        CServerCurrency *c1 = new CServerCurrency();
		c1->m_glid = 645;
		c1->m_valueRatio = 1000;
		cSys->AddTail(c1);
		CServerCurrency *c2 = new CServerCurrency();
		c2->m_glid = 646;
		c2->m_valueRatio = 100;
		cSys->AddTail(c2);
		CServerCurrency *c3 = new CServerCurrency();
		c3->m_glid = 647;
		c3->m_valueRatio = 1;
		cSys->AddTail(c3);



		CInventoryObjList		* m_currentInventory = new CInventoryObjList();

		cSys->AddCurrency(m_gStatus->m_itemManager,m_currentInventory,-1,184);


		delete cSys;
		cSys = 0;*/



	  
		/*POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(!entPosition)
			return;

		CMovementObj *entityPtr = (CMovementObj *) m_gStatus->m_entObjRuntime->GetAt(entPosition);
		
		void * testSize = entityPtr;

		
		/*D3DVECTOR pos;
		entityPtr->List_baseFrame->GetPosition(NULL, &pos);

		pos.y += (entityPtr->List_skeletalObject->m_box.maxY + 1.0f);
     
		if(GetTickCount() - CountGlob > 100)
		{
			CountGlob = GetTickCount();
			m_gStatus->m_textEffectRuntime->SpawnText(	"Hello all, im here on vacation. \nBasically im from florida.",
														3000,
														pos,
														1.0f,
														0.8f,
														1.0f,
														0.8f,
														0.2f,
														0.2f,
														m_gStatus->m_textRenderList,
														m_gStatus->m_symbolMapDB,
														m_gStatus->m_dxRenderer->m_pd3dDevice,
														m_gStatus->m_cgFontList);
		}*/
	}

	if(m_userControls->m_input->m_keyboardKeys[DIK_F4].IsDown())
	{  
		#ifndef _ClientOutput
		if(!m_placementHotKeyEnabled)
			return;

		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(!entPosition)
		{
			MessageBox("no runtime spawned");
			return;
		}
		CMovementObj *entityPtr = (CMovementObj *) m_gStatus->m_entObjRuntime->GetAt(entPosition);

		//LB_GETSELITEMS 
		int groupIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box, LB_GETCURSEL , 0, 0);
		if(groupIndex == -1)
			groupIndex = 0;
		else
		{
			POSITION wldGroupPos = m_gStatus->m_worldGroupList->FindIndex(groupIndex);
			groupIndex = 0;
			if(wldGroupPos)
			{
				CWldGroupObj *wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetAt(wldGroupPos);
				groupIndex = wldPtr->List_groupInt;
			}
		}
		
		for(int loop = 0; loop < m_nodeImporterModule.m_selectionCount; loop++)
			AddNodeByData(groupIndex, m_nodeImporterModule.m_selectionArray[loop], entityPtr, NULL);
		Sleep(200);
		#endif
	}  
}

//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------mouse turn left-------------------------------------------------------//
void EngineXWin::JoystickTurnRight(CMovementCaps *movDB, float Joy1RotY)
{
	if(movDB->List_maxYaw==0)
		return;
	if(Joy1RotY > 0.0f)
	{
		//--------------------actual turn mech--------------------
		movDB->List_curRotY = movDB->List_curRotY + (movDB->List_yawAccel * Joy1RotY);
		if(movDB->List_maxYaw < movDB->List_curRotY)
		movDB->List_curRotY = movDB->List_maxYaw;
	}
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------mouse turn right-------------------------------------------------------//
void EngineXWin::JoystickTurnLeft(CMovementCaps *movDB, float Joy1RotY)
{
	if(movDB->List_maxYaw==0)
		return;
	if(Joy1RotY < 0.0f)
	{
		//--------------------actual turn mech--------------------
		movDB->List_curRotY = movDB->List_curRotY - (movDB->List_yawAccel * -Joy1RotY);
		if(-movDB->List_maxYaw > movDB->List_curRotY)
		movDB->List_curRotY = -movDB->List_maxYaw;
	}
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------mouse turn pull up----------------------------------------------------//
void EngineXWin::JoystickPullUp(CMovementCaps *movDB, float Joy1RotX)
{
	if(movDB->List_maxPitch==0)
		return;
	if(Joy1RotX > 0.0f)
	{
		movDB->List_curRotX = movDB->List_curRotX + (movDB->List_pitchAccel * Joy1RotX);
		if(movDB->List_maxPitch < movDB->List_curRotX)
			movDB->List_curRotX = movDB->List_maxPitch;
	}
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------mouse turn Dive-------------------------------------------------------//
void EngineXWin::JoystickDive(CMovementCaps *movDB, float Joy1RotX)
{
	if(movDB->List_maxPitch==0)
		return;
	if(Joy1RotX < 0.0f)
	{
		movDB->List_curRotX = movDB->List_curRotX - (movDB->List_pitchAccel * -Joy1RotX);
		if(-movDB->List_maxPitch > movDB->List_curRotX)
			movDB->List_curRotX = -movDB->List_maxPitch;
	}
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Function basis decel turn for Mouse-----------------------------------//
void EngineXWin::DecelTurnMouse(CMovementCaps *movDB)
{
	if(movDB->List_curRotY > 0)
		movDB->List_curRotY -= movDB->List_yawDecel;
	else if(movDB->List_curRotY < 0)
		movDB->List_curRotY += movDB->List_yawDecel;
	if(movDB->List_curRotY > 0 && movDB->List_curRotY < (movDB->List_yawDecel+.01f))
		movDB->List_curRotY = 0;
	if(movDB->List_curRotY < 0 && movDB->List_curRotY > -(movDB->List_yawDecel+.01f))
		movDB->List_curRotY = 0;		    
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Function basis decel pitch for Mouse----------------------------------//
void EngineXWin::DecelPitchMouse(CMovementCaps *movDB)
{
	if(movDB->List_curRotX > 0)
		movDB->List_curRotX -= movDB->List_pitchDecel;
	else if(movDB->List_curRotX < 0)
		movDB->List_curRotX += movDB->List_pitchDecel;
	if(movDB->List_curRotX > 0 && movDB->List_curRotX < (movDB->List_pitchDecel+.01f))
		movDB->List_curRotX = 0;
	if(movDB->List_curRotX < 0 && movDB->List_curRotX > -(movDB->List_pitchDecel+.01f))
		movDB->List_curRotX = 0;
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Function basis decel speed no matter what-----------------------------//
void EngineXWin::DecelSpeedManditory(CMovementCaps *movDB)
{
	if(movDB->List_curTransZ > movDB->List_decellerate)
		movDB->List_curTransZ -= movDB->List_decellerate;
	else if(movDB->List_curTransZ < -movDB->List_decellerate)
		movDB->List_curTransZ += movDB->List_decellerate;
	else
		movDB->List_curTransZ = 0.0f;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Physics base----------------------------------------------------------//
void EngineXWin::AccelMove(D3DVECTOR *NewPos, 
						   D3DVECTOR *NewVel,
                           D3DVECTOR CurPos, 
						   D3DVECTOR CurVel,
                           D3DVECTOR CurAccel, 
						   double	 TmSlice)
{
	NewVel->x = CurVel.x + (CurAccel.x * TmSlice);
	NewVel->y = CurVel.y + (CurAccel.y * TmSlice);
	NewVel->z = CurVel.z + (CurAccel.z * TmSlice);

	//Truncate for max speed
	float trackVel = sqrt((NewVel->x * NewVel->x)+(NewVel->y * NewVel->y)+(NewVel->z * NewVel->z));

	//create new position
	NewPos->x = CurPos.x + (TmSlice * (CurVel.x + NewVel->x));
	NewPos->y = CurPos.y + (TmSlice * (CurVel.y + NewVel->y));
	NewPos->z = CurPos.z + (TmSlice * (CurVel.z + NewVel->z));
}