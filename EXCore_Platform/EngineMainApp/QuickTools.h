//------------------------------------------------------------------------------------//
// QuickTools.h - the "Quick" menu in EngineX (easy-editor branch)
//
// Simple one-click tools so you do not have to type X/Y/Z into dialogs:
//   Quick > Play Here              put the character picked in the Entity panel where
//                                  the camera is, and walk around with it (WASD/mouse)
//   Quick > Back To Editor Camera  stop playing, remove the test character, camera back
//   Quick > Where Am I             show the camera position and copy "X Y Z" to the clipboard
//   Quick > Go To...               jump to "X Y Z", "X Z" or an NPC/entity name
//
// Included by enginexwin.cpp (same way as UserInterface.h).
//------------------------------------------------------------------------------------//
#ifndef _ClientOutput

#include <vector>
#include <d3dx9.h>
#include <shlobj.h>
#include <objbase.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")

//------------------------------------------------------------------------------------//
// tiny text prompt (built in memory, so it needs no new dialog resource)
//------------------------------------------------------------------------------------//
struct QuickPromptData
{
	const char * prompt;
	CString    * text;
};

static INT_PTR CALLBACK QuickPromptProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
	{
		QuickPromptData * d = (QuickPromptData *)lParam;
		SetWindowLongPtrA(hDlg, GWLP_USERDATA, (LONG_PTR)d);
		SetDlgItemTextA(hDlg, 100, d->prompt);
		SetDlgItemTextA(hDlg, 101, (const char *)*d->text);
		HWND edit = GetDlgItem(hDlg, 101);
		SetFocus(edit);
		SendMessageA(edit, EM_SETSEL, 0, -1);
		return FALSE;   // we set the focus ourselves
	}
	case WM_COMMAND:
		if(LOWORD(wParam) == IDOK)
		{
			QuickPromptData * d = (QuickPromptData *)GetWindowLongPtrA(hDlg, GWLP_USERDATA);
			char buf[512] = { 0 };
			GetDlgItemTextA(hDlg, 101, buf, sizeof(buf) - 1);
			*d->text = buf;
			EndDialog(hDlg, IDOK);
			return TRUE;
		}
		if(LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

static void QuickAlign(std::vector<WORD> & t) { if(t.size() & 1) t.push_back(0); }   // DWORD-align (2 WORDs)
static void QuickStr(std::vector<WORD> & t, const char * s)
{
	while(*s) t.push_back((WORD)(unsigned char)*s++);
	t.push_back(0);
}
static void QuickItem(std::vector<WORD> & t, DWORD style, short x, short y, short cx, short cy, WORD id, WORD cls, const char * text)
{
	QuickAlign(t);
	t.push_back(LOWORD(style)); t.push_back(HIWORD(style));
	t.push_back(0); t.push_back(0);                         // exStyle
	t.push_back((WORD)x); t.push_back((WORD)y); t.push_back((WORD)cx); t.push_back((WORD)cy);
	t.push_back(id);
	t.push_back(0xFFFF); t.push_back(cls);                  // predefined class
	QuickStr(t, text);
	t.push_back(0);                                         // no creation data
}

// returns TRUE and the typed text on OK
static BOOL QuickPrompt(HWND owner, const char * title, const char * prompt, CString & text)
{
	std::vector<WORD> t;
	DWORD style = DS_MODALFRAME | DS_SETFONT | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU;
	t.push_back(LOWORD(style)); t.push_back(HIWORD(style));
	t.push_back(0); t.push_back(0);                         // exStyle
	t.push_back(4);                                         // 4 controls
	t.push_back(0); t.push_back(0); t.push_back(260); t.push_back(74);
	t.push_back(0); t.push_back(0);                         // no menu, default class
	QuickStr(t, title);
	t.push_back(9); QuickStr(t, "Segoe UI");                // font
	QuickItem(t, WS_CHILD | WS_VISIBLE | SS_LEFT, 8, 6, 244, 26, 100, 0x0082, "");
	QuickItem(t, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL, 8, 34, 244, 13, 101, 0x0081, "");
	QuickItem(t, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON, 146, 54, 50, 14, IDOK, 0x0080, "OK");
	QuickItem(t, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, 202, 54, 50, 14, IDCANCEL, 0x0080, "Cancel");
	QuickPromptData d = { prompt, &text };
	return DialogBoxIndirectParamA(AfxGetInstanceHandle(), (LPCDLGTEMPLATEA)&t[0], owner, QuickPromptProc, (LPARAM)&d) == IDOK;
}

static void QuickCopyToClipboard(HWND owner, const CString & s)
{
	if(!OpenClipboard(owner))
		return;
	EmptyClipboard();
	HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, s.GetLength() + 1);
	if(h)
	{
		char * p = (char *)GlobalLock(h);
		memcpy(p, (const char *)s, s.GetLength() + 1);
		GlobalUnlock(h);
		if(!SetClipboardData(CF_TEXT, h))
			GlobalFree(h);
	}
	CloseClipboard();
}

//------------------------------------------------------------------------------------//
// what the camera is following right now (the editor camera or your test character)
//------------------------------------------------------------------------------------//
CMovementObj * EngineXWin::QuickCameraEntity()
{
	if(!m_gStatus || !m_gStatus->m_entObjRuntime || !m_gStatus->m_cameraModule)
		return NULL;
	return m_gStatus->m_entObjRuntime->GetObjByTrace(m_gStatus->m_cameraModule->m_followTrace);
}

static void QuickMoveEntity(CMovementObj * ent, D3DVECTOR pos)
{
	if(!ent || !ent->List_baseFrame)
		return;
	ent->List_baseFrame->SetPosition(NULL, pos);
	ent->List_lastPosition = pos;   // so movement/collision does not pull it back
}

//------------------------------------------------------------------------------------//
// Quick > Play Here
//------------------------------------------------------------------------------------//
void EngineXWin::OnQuickPlayHere()
{
	CMovementObj * cam = QuickCameraEntity();
	if(!cam || !cam->List_baseFrame)
	{
		MessageBox("Load a zone first (File > Load Zone).", "Play Here", MB_OK | MB_ICONINFORMATION);
		return;
	}

	// which character: the one picked in the Entity panel's Database tree
	HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem(IDC_TREE_ENTITIES, &treeHwnd);
	HTREEITEM item = treeHwnd ? TreeView_GetSelection(treeHwnd) : NULL;
	CMovementObj * dbPtr = NULL;
	if(item)
	{
		CString pick = GetTreeItemText(IDC_TREE_ENTITIES, &m_entityDlgBar, item, 50);
		pick.Replace("TRC_", "");
		dbPtr = m_gStatus->m_entObjList->GetByID(atoi(pick));
	}
	if(!dbPtr)
	{
		MessageBox("Pick the character first:\n\nopen the Entity panel and click a character in the Database list\n"
		           "(for example the Human Male), then use Quick > Play Here again.",
		           "Play Here", MB_OK | MB_ICONINFORMATION);
		return;
	}

	// if we are already playing, take the old test character away first
	if(m_quickPlayTrace >= 0)
		QuickRemoveTestCharacter();

	D3DVECTOR pos;
	cam->List_baseFrame->GetPosition(NULL, &pos);
	pos.y += 10.0f;   // a little above, so it drops onto the floor instead of into it

	D3DMATRIX where;
	CFrameObj * tempFrame = new CFrameObj(NULL);
	tempFrame->SetPosition(NULL, pos);
	tempFrame->GetTransform(NULL, &where);
	tempFrame->SafeDelete();
	delete tempFrame;

	int editorTrace = m_gStatus->m_cameraModule->m_followTrace;
	BOOL ok = m_gStatus->ActivateMovementEntity(NULL, 0, where, 0, 0, dbPtr->m_uniqueId, 0, 0, 0,
	                                            FALSE, TRUE, FALSE, -1, -1, NULL, NULL,
	                                            dbPtr->m_primaryPhysicsOverd, -1,
	                                            m_gStatus->m_collisionBase, m_gStatus->m_collisionDatabase,
	                                            m_gStatus->m_entObjRuntime, m_gStatus->m_currentWorkingFile);
	POSITION head = m_gStatus->m_entObjRuntime->GetHeadPosition();
	CMovementObj * player = (ok && head) ? (CMovementObj *)m_gStatus->m_entObjRuntime->GetAt(head) : NULL;
	if(!player)
	{
		MessageBox("Could not place that character here.", "Play Here", MB_OK | MB_ICONWARNING);
		return;
	}

	if(m_quickEditorTrace < 0)
		m_quickEditorTrace = editorTrace;   // remember the editor camera to come back to
	m_quickPlayTrace = player->List_traceNumber;

	// take control of it (same as double-clicking it in the runtime list)
	player->List_controlType = 0;
	if(player->List_energy <= 0)
		player->List_energy = player->List_maxEnergy > 0 ? player->List_maxEnergy : 100;
	m_gStatus->m_cameraModule->m_followTrace = player->List_traceNumber;
	m_gStatus->m_globalAvVar = TRUE;
	if(m_gStatus->m_dxRenderer)
		m_gStatus->m_dxRenderer->DisableMouse();

	InListRuntimeEntities(0);
	if(m_quickOldTitle.IsEmpty())
		GetWindowText(m_quickOldTitle);
	SetWindowText(m_quickOldTitle + "  -  PLAYING (Quick > Back To Editor Camera to stop, F9 = mouse)");
}

//------------------------------------------------------------------------------------//
// take the Play Here test character out of the zone (it is never meant to be saved)
//------------------------------------------------------------------------------------//
void EngineXWin::QuickRemoveTestCharacter()
{
	if(m_quickPlayTrace < 0)
		return;
	for(POSITION pos = m_gStatus->m_entObjRuntime->GetHeadPosition(); pos != NULL;)
	{
		POSITION at = pos;
		CMovementObj * e = (CMovementObj *)m_gStatus->m_entObjRuntime->GetNext(pos);
		if(e && e->List_traceNumber == m_quickPlayTrace)
		{
			m_gStatus->m_entObjRuntime->RemoveAt(at);
			m_gStatus->DestroyRuntimeEntity(e, m_gStatus->m_spawnGeneratorDB, m_gStatus->m_entObjRuntime);
			break;
		}
	}
	m_quickPlayTrace = -1;
}

//------------------------------------------------------------------------------------//
// Quick > Back To Editor Camera
//------------------------------------------------------------------------------------//
void EngineXWin::OnQuickBackToEditor()
{
	if(m_quickPlayTrace < 0)
	{
		MessageBox("You are not playing a test character right now.", "Back To Editor Camera", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if(m_quickEditorTrace >= 0)
		m_gStatus->m_cameraModule->m_followTrace = m_quickEditorTrace;
	QuickRemoveTestCharacter();
	m_quickEditorTrace = -1;
	if(m_gStatus->m_dxRenderer)
		m_gStatus->m_dxRenderer->EnableMouse();
	InListRuntimeEntities(0);
	if(!m_quickOldTitle.IsEmpty())
		SetWindowText(m_quickOldTitle);
	m_quickOldTitle.Empty();
}

//------------------------------------------------------------------------------------//
// Quick > Where Am I
//------------------------------------------------------------------------------------//
void EngineXWin::OnQuickWhereAmI()
{
	CMovementObj * cam = QuickCameraEntity();
	if(!cam || !cam->List_baseFrame)
	{
		MessageBox("Load a zone first (File > Load Zone).", "Where Am I", MB_OK | MB_ICONINFORMATION);
		return;
	}
	D3DVECTOR pos;
	cam->List_baseFrame->GetPosition(NULL, &pos);
	CString xyz;
	xyz.Format("%.0f %.0f %.0f", pos.x, pos.y, pos.z);
	QuickCopyToClipboard(GetSafeHwnd(), xyz);
	CString msg;
	msg.Format("Zone: %s\n\nX = %.0f\nY = %.0f   (height)\nZ = %.0f\n\n"
	           "\"%s\" is copied - paste it into Quick > Go To, or anywhere you need X Y Z.\n"
	           "(The in-game minimap shows X, Z.)",
	           (const char *)m_gStatus->m_currentWorkingFile, pos.x, pos.y, pos.z, (const char *)xyz);
	MessageBox(msg, "Where Am I", MB_OK | MB_ICONINFORMATION);
}

//------------------------------------------------------------------------------------//
// Quick > Go To...   "X Y Z", "X Z" (keeps your height) or a name ("Tidi", "Portal")
//------------------------------------------------------------------------------------//
void EngineXWin::OnQuickGoTo()
{
	CMovementObj * cam = QuickCameraEntity();
	if(!cam || !cam->List_baseFrame)
	{
		MessageBox("Load a zone first (File > Load Zone).", "Go To", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CString text = m_quickLastGoTo;
	if(!QuickPrompt(GetSafeHwnd(), "Go To",
	                "Type X Y Z,  or X Z (keeps your height),  or a name (NPC, portal...):", text))
		return;
	text.Trim();
	if(text.IsEmpty())
		return;
	m_quickLastGoTo = text;

	D3DVECTOR cur, dest;
	cam->List_baseFrame->GetPosition(NULL, &cur);
	dest = cur;

	CString nums = text;
	nums.Replace(",", " ");
	float a = 0, b = 0, c = 0;
	int n = sscanf_s((const char *)nums, "%f %f %f", &a, &b, &c);
	if(n == 3)
	{
		dest.x = a; dest.y = b; dest.z = c;
	}
	else if(n == 2)
	{
		dest.x = a; dest.z = b;   // same as the minimap: X, Z
	}
	else
	{// find a placed entity whose name contains the text (not the camera itself)
		CMovementObj * found = NULL;
		CString want = text;
		want.MakeLower();
		for(POSITION pos = m_gStatus->m_entObjRuntime->GetHeadPosition(); pos != NULL;)
		{
			CMovementObj * e = (CMovementObj *)m_gStatus->m_entObjRuntime->GetNext(pos);
			if(!e || e == cam || !e->List_baseFrame)
				continue;
			CString nm = e->List_name;
			nm.MakeLower();
			if(nm.Find(want) >= 0)
			{
				found = e;
				break;
			}
		}
		if(!found)
		{
			CString msg;
			msg.Format("Nothing placed in this zone is called \"%s\".\n\nTry part of the name, or type X Y Z.", (const char *)text);
			MessageBox(msg, "Go To", MB_OK | MB_ICONINFORMATION);
			return;
		}
		found->List_baseFrame->GetPosition(NULL, &dest);
		dest.x += 25.0f;   // stand a little next to it, not inside it
		dest.y += 10.0f;
	}
	QuickMoveEntity(cam, dest);
}

//------------------------------------------------------------------------------------//
// Quick > Export Character For Blender (glTF)
//   pick a character in the Entity panel's Database list, choose where to save.
//   Writes <name>.gltf + .bin (bones, skinned body parts) and the textures as PNG.
//   In Blender: File > Import > glTF 2.0.
//------------------------------------------------------------------------------------//
static BOOL QuickSaveTexturePng(GlobalStatusVariables * gs, const CString & texBase, const CString & outPng)
{
	// 1) the texture as the engine has it loaded
	if(gs->m_textureDataBase)
	{
		for(POSITION pos = gs->m_textureDataBase->GetHeadPosition(); pos != NULL;)
		{
			CTextureDBObj * t = (CTextureDBObj *)gs->m_textureDataBase->GetNext(pos);
			if(!t || !t->m_textureObject) continue;
			CString nm = t->List_textureName;
			int dot = nm.ReverseFind('.');
			if(dot > 0) nm = nm.Left(dot);
			if(nm.CompareNoCase(texBase) != 0) continue;
			IDirect3DTexture9 * surf = t->m_textureObject->GetSurface();
			if(surf && SUCCEEDED(D3DXSaveTextureToFileA(outPng, D3DXIFF_PNG, surf, NULL)))
				return TRUE;
		}
	}
	// 2) a loose file in MapsModels
	if(gs->m_dxRenderer && gs->m_dxRenderer->m_pd3dDevice)
	{
		static const char * exts[] = { ".png", ".tga", ".dds", ".bmp", ".jpg" };
		for(int e = 0; e < 5; e++)
		{
			CString src = gs->GL_workingDirectory + "\\MapsModels\\" + texBase + exts[e];
			if(GetFileAttributesA(src) == INVALID_FILE_ATTRIBUTES) continue;
			IDirect3DTexture9 * tex = NULL;
			if(SUCCEEDED(D3DXCreateTextureFromFileA(gs->m_dxRenderer->m_pd3dDevice, src, &tex)) && tex)
			{
				HRESULT hr = D3DXSaveTextureToFileA(outPng, D3DXIFF_PNG, tex, NULL);
				tex->Release();
				if(SUCCEEDED(hr)) return TRUE;
			}
		}
	}
	return FALSE;
}

void EngineXWin::OnQuickExportCharacter() { QuickExportCharacterImpl(FALSE); }
void EngineXWin::OnQuickExportOutfits()   { QuickExportCharacterImpl(TRUE); }
void EngineXWin::QuickExportCharacterImpl(BOOL allOutfits)
{
	HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem(IDC_TREE_ENTITIES, &treeHwnd);
	HTREEITEM item = treeHwnd ? TreeView_GetSelection(treeHwnd) : NULL;
	CMovementObj * dbPtr = NULL;
	if(item)
	{
		CString pick = GetTreeItemText(IDC_TREE_ENTITIES, &m_entityDlgBar, item, 50);
		pick.Replace("TRC_", "");
		dbPtr = m_gStatus->m_entObjList->GetByID(atoi(pick));
	}
	if(!dbPtr || !dbPtr->List_skeletalObject)
	{
		MessageBox("Pick the character first:\n\nopen the Entity panel and click a character in the Database list,\n"
		           "then use Quick > Export Character For Blender again.", "Export Character", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CString suggested = dbPtr->List_name;
	suggested.Replace(" ", "_");
	suggested.Replace("/", "_");
	suggested.Replace("\\", "_");
	if(suggested.IsEmpty()) suggested.Format("character_%d", dbPtr->m_uniqueId);
	if(allOutfits) suggested += "_outfits";
	CFileDialog dlg(FALSE, "gltf", suggested + ".gltf", OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
	                "glTF for Blender (*.gltf)|*.gltf||", this);
	if(dlg.DoModal() != IDOK)
		return;
	CString path = dlg.GetPathName();

	CStringArray texNames;
	CString err;
	if(!dbPtr->List_skeletalObject->ExportToGltf(path, texNames, err, allOutfits))
	{
		MessageBox("Export failed:\n\n" + err, "Export Character", MB_OK | MB_ICONWARNING);
		return;
	}
	CString dir = path.Left(path.ReverseFind('\\'));
	CString missing;
	int saved = 0;
	for(int t = 0; t < texNames.GetSize(); t++)
	{
		if(QuickSaveTexturePng(m_gStatus, texNames[t], dir + "\\" + texNames[t] + ".png"))
			saved++;
		else
			missing += "\n   " + texNames[t];
	}
	CString msg;
	msg.Format("Saved %s\n\n%d texture(s) saved as PNG next to it.%s\n\n"
	           "In Blender: File > Import > glTF 2.0 and pick the .gltf.",
	           (const char *)path, saved,
	           missing.IsEmpty() ? "" : (const char *)("\n\nCould not find these textures:" + missing));
	MessageBox(msg, "Export Character", MB_OK | MB_ICONINFORMATION);
}

//------------------------------------------------------------------------------------//
// Quick > Import Character From Blender (glTF)
//   pick the SAME character in the Entity panel's Database list, then the .gltf/.glb
//   you saved from Blender. The body parts are replaced in the editor; use
//   "Save Single Entity" / "Save Database" in the Entity panel to keep them.
//------------------------------------------------------------------------------------//
void EngineXWin::OnQuickImportCharacter()
{
	HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem(IDC_TREE_ENTITIES, &treeHwnd);
	HTREEITEM item = treeHwnd ? TreeView_GetSelection(treeHwnd) : NULL;
	CMovementObj * dbPtr = NULL;
	if(item)
	{
		CString pick = GetTreeItemText(IDC_TREE_ENTITIES, &m_entityDlgBar, item, 50);
		pick.Replace("TRC_", "");
		dbPtr = m_gStatus->m_entObjList->GetByID(atoi(pick));
	}
	if(!dbPtr || !dbPtr->List_skeletalObject)
	{
		MessageBox("Pick the character first:\n\nopen the Entity panel and click the character in the Database list\n"
		           "(the same one you exported), then use Quick > Import Character From Blender again.",
		           "Import Character", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CFileDialog dlg(TRUE, "gltf", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
	                "Blender export (*.gltf;*.glb;*.fbx)|*.gltf;*.glb;*.fbx|All files (*.*)|*.*||", this);
	if(dlg.DoModal() != IDOK)
		return;
	CString msg;
	msg.Format("Replace the body of \"%s\" with\n%s ?\n\n"
	           "This only changes the editor's copy. Nothing is saved until you press\n"
	           "\"Save Single Entity\" or \"Save Database\" in the Entity panel\n"
	           "(back up your entity files first).",
	           (const char *)dbPtr->List_name, (const char *)dlg.GetPathName());
	if(MessageBox(msg, "Import Character", MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
		return;
	CString report;
	LPDIRECT3DDEVICE9 dev = m_gStatus->m_dxRenderer ? m_gStatus->m_dxRenderer->m_pd3dDevice : NULL;
	BOOL ok = dbPtr->List_skeletalObject->ImportFromGltf(dlg.GetPathName(), dev, report);
	if(ok) m_gStatus->SkeletalTextureReassignment(dbPtr->List_skeletalObject);   // load the new texture(s) now
	if(ok)
		report += "\n\nUse Quick > Play Here with this character to see it walk.\n"
		          "(Characters already placed keep the old body until placed again.)";
	MessageBox(report, "Import Character", MB_OK | (ok ? MB_ICONINFORMATION : MB_ICONWARNING));
}

void EngineXWin::OnQuickGiveTopsJiggle()
{
	HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem(IDC_TREE_ENTITIES, &treeHwnd);
	HTREEITEM item = treeHwnd ? TreeView_GetSelection(treeHwnd) : NULL;
	CMovementObj * dbPtr = NULL;
	if(item)
	{
		CString pick = GetTreeItemText(IDC_TREE_ENTITIES, &m_entityDlgBar, item, 50);
		pick.Replace("TRC_", "");
		dbPtr = m_gStatus->m_entObjList->GetByID(atoi(pick));
	}
	if(!dbPtr || !dbPtr->List_skeletalObject)
	{
		MessageBox("Pick the character first:\n\nopen the Entity panel and click the character in the Database list,\n"
		           "then use Quick > Give All Tops Jiggle again.",
		           "Give All Tops Jiggle", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CString report;
	BOOL ok = dbPtr->List_skeletalObject->GiveTopsJiggle(report);
	if(ok)
		report += "\n\nTest it with Quick > Play Here.\n"
		          "If it looks right press \"Save Single Entity\" in the Entity panel to keep it\n"
		          "(back up your entity files first).";
	MessageBox(report, "Give All Tops Jiggle", MB_OK | (ok ? MB_ICONINFORMATION : MB_ICONWARNING));
}

//------------------------------------------------------------------------------------//
// Quick > Copy Character As New Entity
//   pick a character in the Database list; makes a full, separate copy of it (the same
//   save/load the SIE buttons use, so nothing is shared with the original), gives it the
//   next free TRC number and the name you type. Use Save Database to keep it.
//------------------------------------------------------------------------------------//
void EngineXWin::OnQuickCopyEntity()
{
	HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem(IDC_TREE_ENTITIES, &treeHwnd);
	HTREEITEM item = treeHwnd ? TreeView_GetSelection(treeHwnd) : NULL;
	CMovementObj * src = NULL;
	if(item)
	{
		CString pick = GetTreeItemText(IDC_TREE_ENTITIES, &m_entityDlgBar, item, 50);
		pick.Replace("TRC_", "");
		src = m_gStatus->m_entObjList->GetByID(atoi(pick));
	}
	if(!src)
	{
		MessageBox("Pick the character to copy first:\n\nclick its TRC_ number in the Entity panel's Database list,\n"
		           "then use Quick > Copy Character As New Entity again.", "Copy Character", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CString newName = src->List_name + " Female";
	if(!QuickPrompt(GetSafeHwnd(), "Copy Character", "Name for the new character:", newName) || newName.IsEmpty())
		return;
	CMovementObj * copy = NULL;
	try
	{
		CMemFile mem;
		{
			CArchive outAr(&mem, CArchive::store);
			outAr << src;
			outAr.Close();
		}
		mem.SeekToBegin();
		CArchive inAr(&mem, CArchive::load);
		inAr >> copy;
		inAr.Close();
	}
	catch(CException * e)
	{
		e->Delete();
		copy = NULL;
	}
	if(!copy)
	{
		MessageBox("Could not copy this character.", "Copy Character", MB_OK | MB_ICONWARNING);
		return;
	}
	copy->List_name = newName;
	copy->List_traceNumber = m_gStatus->GlobalTraceNumber++;
	copy->m_uniqueId = m_gStatus->m_entObjList->GetNextHighID();
	m_gStatus->m_entObjList->AddTail(copy);
	if(copy->List_skeletalObject)
	{
		copy->List_skeletalObject->m_SkeletalName = newName;
		InitFXSahdersOnSkeletal(copy->List_skeletalObject);
		m_gStatus->SkeletalTextureReassignmentWCompile(copy->List_skeletalObject);
	}
	EnitityInList();
	CString msg;
	msg.Format("Made TRC_%d \"%s\" (a copy of TRC_%d).\n\n"
	           "Click TRC_%d, then Quick > Import Character From Blender to give it a new body.\n"
	           "Press \"Save Database\" to keep the new character (back up your files first).",
	           copy->m_uniqueId, (const char *)newName, src->m_uniqueId, copy->m_uniqueId);
	MessageBox(msg, "Copy Character", MB_OK | MB_ICONINFORMATION);
}

//------------------------------------------------------------------------------------//
// Quick > Give Race The Same Items As...
//   every armor / weapon / jetpack says which races (TRC numbers) can use it and how it
//   shows on them. This copies all of one race's entries to another race, so a copied
//   character (like a female made from the male frog) can wear and hold the same things.
//   Then it saves GameFiles\EARevolution.APX (the old one is kept as a .bak).
//------------------------------------------------------------------------------------//
void EngineXWin::OnQuickCopyItemRace()
{
	CItemManager * items = m_gStatus->m_itemManager;
	if(!items || !items->m_listArray || items->m_indexedVarCount == 0)
	{
		MessageBox("No items are loaded.\n\nLoad them first (Item Manager > Load > EARevolution.APX), then try again.",
		           "Give Race Items", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CString toText = "115";
	HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem(IDC_TREE_ENTITIES, &treeHwnd);
	HTREEITEM picked = treeHwnd ? TreeView_GetSelection(treeHwnd) : NULL;
	if(picked)
	{
		CString pick = GetTreeItemText(IDC_TREE_ENTITIES, &m_entityDlgBar, picked, 50);
		if(pick.Find("TRC_") == 0) { pick.Replace("TRC_", ""); toText = pick; }
	}
	CString fromText = "18";
	if(!QuickPrompt(GetSafeHwnd(), "Give Race Items", "Copy items FROM this TRC number (the race that already has them):", fromText) || fromText.IsEmpty())
		return;
	if(!QuickPrompt(GetSafeHwnd(), "Give Race Items", "Give them TO this TRC number (the new race):", toText) || toText.IsEmpty())
		return;
	int fromRace = atoi(fromText), toRace = atoi(toText);
	if(fromRace == toRace || toRace < 0)
		return;

	int itemsSeen = 0, itemsGiven = 0, itemsHad = 0, refsAdded = 0;
	for(unsigned int cell = 0; cell < items->m_cellCount; cell++)
	{
		for(CAxLinkObj * pos = items->m_listArray[0].m_axList[cell].GetHeadPosition_unsafe(); pos != NULL;)
		{
			CItemObj * item = (CItemObj *)items->m_listArray[0].m_axList[cell].GetNext_unsafe(&pos);
			if(!item || !item->m_attachmentRefList) continue;
			itemsSeen++;
			std::vector<CAttachmentRefObj *> fromRefs;
			bool hasTo = false;
			for(POSITION p = item->m_attachmentRefList->GetHeadPosition(); p != NULL;)
			{
				CAttachmentRefObj * ref = (CAttachmentRefObj *)item->m_attachmentRefList->GetNext(p);
				if(!ref) continue;
				if(ref->m_raceBind == toRace) hasTo = true;
				if(ref->m_raceBind == fromRace) fromRefs.push_back(ref);
			}
			if(fromRefs.empty()) continue;
			if(hasTo) { itemsHad++; continue; }
			for(size_t i = 0; i < fromRefs.size(); i++)
			{
				CAttachmentRefObj * add = new CAttachmentRefObj();
				add->m_attachmentType = fromRefs[i]->m_attachmentType;
				add->m_raceBind       = toRace;
				add->m_dimIndex       = fromRefs[i]->m_dimIndex;
				add->m_dimSwitchTo    = fromRefs[i]->m_dimSwitchTo;
				add->m_fireQueOvrd    = fromRefs[i]->m_fireQueOvrd;
				item->m_attachmentRefList->AddTail(add);
				refsAdded++;
			}
			itemsGiven++;
		}
	}
	if(itemsGiven == 0)
	{
		CString msg;
		msg.Format("Nothing to copy: no item has TRC_%d entries that TRC_%d is missing.\n(%d items checked, %d already had TRC_%d.)",
		           fromRace, toRace, itemsSeen, itemsHad, toRace);
		MessageBox(msg, "Give Race Items", MB_OK | MB_ICONINFORMATION);
		return;
	}
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	CString apx = CString(buffer) + "\\GameFiles\\EARevolution.APX";
	CString bak = apx + ".bak-before-race-items";
	CopyFile(apx, bak, TRUE);   // keep the first backup, never overwrite it
	BOOL saved = items->Save(apx.GetBuffer());
	apx.ReleaseBuffer();
	CString msg;
	msg.Format("TRC_%d can now use %d items the same way TRC_%d does (armor, weapons, jetpacks...).\n"
	           "%d items already had TRC_%d and were left alone.\n\n%s\n%s\n\nOld file kept as:\n%s",
	           toRace, itemsGiven, fromRace, itemsHad, toRace,
	           saved ? "Saved:" : "SAVE FAILED - nothing was written to:", (const char *)apx, (const char *)bak);
	MessageBox(msg, "Give Race Items", MB_OK | (saved ? MB_ICONINFORMATION : MB_ICONWARNING));
}


//------------------------------------------------------------------------------------//
// Quick > Send To Game
// Copies the saved entity, item, and menu databases, the stock Static.TXB bank
// when present, and any texture files that are missing or changed under MapsModels.
// First use asks for the Core folder and EA server root; those choices are saved
// in the editor profile, so normal use is one menu action.
//------------------------------------------------------------------------------------//
struct QuickDeployFile
{
    CString sourcePath;
    CString relativePath;
};

struct QuickDeployStats
{
    int copied;
    int unchanged;
    int backups;
    int failures;
    CString errors;
};

static CString QuickDeployJoinPath(const CString & base, const CString & child)
{
    CString left = base;
    CString right = child;
    const char slash = (char)92;
    left.Replace('/', slash);
    right.Replace('/', slash);
    while(left.GetLength() > 3 && left.GetAt(left.GetLength() - 1) == slash)
        left = left.Left(left.GetLength() - 1);
    while(right.GetLength() > 0 && right.GetAt(0) == slash)
        right = right.Mid(1);
    if(left.IsEmpty())
        return right;
    if(left.GetAt(left.GetLength() - 1) != slash)
        left += slash;
    return left + right;
}

static BOOL QuickDeployDirectoryExists(const CString & path)
{
    DWORD attributes = GetFileAttributesA(path);
    return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

static BOOL QuickDeployFileExists(const CString & path)
{
    DWORD attributes = GetFileAttributesA(path);
    return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

static BOOL QuickDeployEnsureDirectory(const CString & path)
{
    if(QuickDeployDirectoryExists(path))
        return TRUE;

    CString normalized = path;
    normalized.Replace('/', (char)92);
    int slash = normalized.ReverseFind((char)92);
    if(slash < 0)
        return FALSE;

    CString parent;
    if(slash == 2 && normalized.GetLength() >= 3 && normalized.GetAt(1) == ':')
        parent = normalized.Left(3); // C:\
    else
        parent = normalized.Left(slash);
    if(parent.IsEmpty() || parent.CompareNoCase(normalized) == 0)
        return FALSE;
    if(!QuickDeployDirectoryExists(parent) && !QuickDeployEnsureDirectory(parent))
        return FALSE;

    if(CreateDirectoryA(normalized, NULL))
        return TRUE;
    return GetLastError() == ERROR_ALREADY_EXISTS && QuickDeployDirectoryExists(normalized);
}

static CString QuickDeployDirectoryPart(const CString & path)
{
    int slash = path.ReverseFind((char)92);
    int forwardSlash = path.ReverseFind('/');
    if(forwardSlash > slash)
        slash = forwardSlash;
    if(slash < 0)
        return CString(".");
    if(slash == 2 && path.GetLength() >= 3 && path.GetAt(1) == ':')
        return path.Left(3);
    return path.Left(slash);
}

static BOOL QuickDeployFilesEqual(const CString & firstPath, const CString & secondPath)
{
    HANDLE first = CreateFileA(firstPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                               NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if(first == INVALID_HANDLE_VALUE)
        return FALSE;
    HANDLE second = CreateFileA(secondPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if(second == INVALID_HANDLE_VALUE)
    {
        CloseHandle(first);
        return FALSE;
    }

    LARGE_INTEGER firstSize, secondSize;
    if(!GetFileSizeEx(first, &firstSize) || !GetFileSizeEx(second, &secondSize) ||
       firstSize.QuadPart != secondSize.QuadPart)
    {
        CloseHandle(second);
        CloseHandle(first);
        return FALSE;
    }

    std::vector<BYTE> firstBuffer(64 * 1024);
    std::vector<BYTE> secondBuffer(64 * 1024);
    BOOL equal = TRUE;
    for(;;)
    {
        DWORD firstRead = 0, secondRead = 0;
        if(!ReadFile(first, &firstBuffer[0], (DWORD)firstBuffer.size(), &firstRead, NULL) ||
           !ReadFile(second, &secondBuffer[0], (DWORD)secondBuffer.size(), &secondRead, NULL) ||
           firstRead != secondRead)
        {
            equal = FALSE;
            break;
        }
        if(firstRead == 0)
            break;
        if(memcmp(&firstBuffer[0], &secondBuffer[0], firstRead) != 0)
        {
            equal = FALSE;
            break;
        }
    }

    CloseHandle(second);
    CloseHandle(first);
    return equal;
}

static int CALLBACK QuickDeployBrowseCallback(HWND dialog, UINT message, LPARAM, LPARAM initialPath)
{
    if(message == BFFM_INITIALIZED && initialPath)
    {
        const char * path = (const char *)initialPath;
        if(path[0])
            SendMessageA(dialog, BFFM_SETSELECTIONA, TRUE, (LPARAM)path);
    }
    return 0;
}

static CString QuickDeployBrowseForFolder(HWND owner, const char * title, const CString & initialPath)
{
    BROWSEINFOA info;
    ZeroMemory(&info, sizeof(info));
    char displayName[MAX_PATH] = { 0 };
    info.hwndOwner = owner;
    info.pszDisplayName = displayName;
    info.lpszTitle = title;
    info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
    info.lpfn = QuickDeployBrowseCallback;
    info.lParam = (LPARAM)(const char *)initialPath;

    LPITEMIDLIST item = SHBrowseForFolderA(&info);
    if(!item)
        return CString();
    char selected[MAX_PATH] = { 0 };
    BOOL gotPath = SHGetPathFromIDListA(item, selected);
    CoTaskMemFree(item);
    return gotPath ? CString(selected) : CString();
}

static CString QuickDeployReadFolderProfile(LPCTSTR key)
{
    CWinApp * app = AfxGetApp();
    return app ? app->GetProfileString(_T("QuickTools"), key, _T("")) : CString();
}

static void QuickDeployWriteFolderProfile(LPCTSTR key, const CString & path)
{
    CWinApp * app = AfxGetApp();
    if(app)
        app->WriteProfileString(_T("QuickTools"), key, path);
}

static BOOL QuickDeployChooseFolder(HWND owner, LPCTSTR profileKey, const char * title,
                                    const CString & initialPath, BOOL requireGameFiles,
                                    BOOL forceBrowse, CString & selectedPath)
{
    CString configured = QuickDeployReadFolderProfile(profileKey);
    if(!forceBrowse && QuickDeployDirectoryExists(configured) &&
       (!requireGameFiles || QuickDeployDirectoryExists(QuickDeployJoinPath(configured, "GameFiles"))))
    {
        selectedPath = configured;
        return TRUE;
    }

    CString startAt = configured.IsEmpty() ? initialPath : configured;
    if(!QuickDeployDirectoryExists(startAt))
        startAt.Empty();
    selectedPath = QuickDeployBrowseForFolder(owner, title, startAt);
    if(selectedPath.IsEmpty())
        return FALSE;
    if(!QuickDeployDirectoryExists(selectedPath))
    {
        AfxMessageBox("Please choose an existing folder.", MB_ICONWARNING);
        selectedPath.Empty();
        return FALSE;
    }
    if(requireGameFiles && !QuickDeployDirectoryExists(QuickDeployJoinPath(selectedPath, "GameFiles")))
    {
        AfxMessageBox("Choose the Core game folder that contains the GameFiles folder.", MB_ICONWARNING);
        selectedPath.Empty();
        return FALSE;
    }

    QuickDeployWriteFolderProfile(profileKey, selectedPath);
    return TRUE;
}

static CString QuickDeployResolvePatchFolder(const CString & serverRoot)
{
    CString configured = "Patch";
    CString configPath = QuickDeployJoinPath(serverRoot, "ContentServer.cfg");
    FILE * config = NULL;
    if(fopen_s(&config, (const char *)configPath, "r") == 0 && config)
    {
        char line[512];
        while(fgets(line, sizeof(line), config))
        {
            if(_strnicmp(line, "PatchFolder=", 12) == 0)
            {
                configured = line + 12;
                configured.Trim();
                if(configured.GetLength() >= 2 && configured.GetAt(0) == '"' &&
                   configured.GetAt(configured.GetLength() - 1) == '"')
                    configured = configured.Mid(1, configured.GetLength() - 2);
                break;
            }
        }
        fclose(config);
    }
    if(configured.IsEmpty())
        configured = "Patch";

    BOOL absolutePath = configured.Find(':') == 1 ||
        (configured.GetLength() >= 2 && configured.GetAt(0) == (char)92 && configured.GetAt(1) == (char)92);
    return absolutePath ? configured : QuickDeployJoinPath(serverRoot, configured);
}

static BOOL QuickDeployIsTextureFile(const CString & fileName)
{
    int dot = fileName.ReverseFind('.');
    if(dot < 0)
        return FALSE;
    CString ext = fileName.Mid(dot);
    ext.MakeLower();
    return ext == ".bmp" || ext == ".dds" || ext == ".gif" || ext == ".jpeg" ||
           ext == ".jpg" || ext == ".png" || ext == ".tga" || ext == ".tif" || ext == ".tiff";
}

static void QuickDeployCollectTextures(const CString & directory, const CString & relativePrefix,
                                      std::vector<QuickDeployFile> & files)
{
    CString pattern = QuickDeployJoinPath(directory, "*");
    WIN32_FIND_DATAA data;
    HANDLE find = FindFirstFileA(pattern, &data);
    if(find == INVALID_HANDLE_VALUE)
        return;
    do
    {
        CString name = data.cFileName;
        if(name == "." || name == "..")
            continue;
        if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if(data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
                continue; // don't recurse through junctions or symlinks
            QuickDeployCollectTextures(QuickDeployJoinPath(directory, name),
                                       QuickDeployJoinPath(relativePrefix, name), files);
        }
        else if(QuickDeployIsTextureFile(name))
        {
            QuickDeployFile entry;
            entry.sourcePath = QuickDeployJoinPath(directory, name);
            entry.relativePath = QuickDeployJoinPath(relativePrefix, name);
            files.push_back(entry);
        }
    } while(FindNextFileA(find, &data));
    FindClose(find);
}

static void QuickDeployMakeTimestamp(CString & stamp)
{
    SYSTEMTIME now;
    GetLocalTime(&now);
    stamp.Format("%04d%02d%02d-%02d%02d%02d-%03d", now.wYear, now.wMonth, now.wDay,
                 now.wHour, now.wMinute, now.wSecond, now.wMilliseconds);
}

static void QuickDeployRecordError(QuickDeployStats & stats, const CString & error)
{
    stats.failures++;
    if(stats.failures <= 8)
        stats.errors += "\n  " + error;
}

static BOOL QuickDeployOneFile(const QuickDeployFile & entry, const CString & targetRoot,
                               const CString & stamp, QuickDeployStats & stats)
{
    CString destination = QuickDeployJoinPath(targetRoot, entry.relativePath);
    DWORD attributes = GetFileAttributesA(destination);
    BOOL destinationExists = attributes != INVALID_FILE_ATTRIBUTES &&
                             (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
    if(attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        CString error = "A folder already exists where this file should go: " + destination;
        QuickDeployRecordError(stats, error);
        return FALSE;
    }
    if(destinationExists && QuickDeployFilesEqual(entry.sourcePath, destination))
    {
        stats.unchanged++;
        return TRUE;
    }

    CString destinationDirectory = QuickDeployDirectoryPart(destination);
    if(!QuickDeployEnsureDirectory(destinationDirectory))
    {
        QuickDeployRecordError(stats, "Could not create folder: " + destinationDirectory);
        return FALSE;
    }

    if(destinationExists)
    {
        CString backupRoot = QuickDeployJoinPath(targetRoot, QuickDeployJoinPath("_EA_Backups", stamp));
        CString backupPath = QuickDeployJoinPath(backupRoot, entry.relativePath);
        if(!QuickDeployEnsureDirectory(QuickDeployDirectoryPart(backupPath)))
        {
            QuickDeployRecordError(stats, "Could not create backup folder for: " + destination);
            return FALSE;
        }
        if(!CopyFileA(destination, backupPath, TRUE))
        {
            CString error;
            error.Format("Could not back up %s (Windows error %lu).",
                         (const char *)destination, GetLastError());
            QuickDeployRecordError(stats, error);
            return FALSE;
        }
        stats.backups++;
    }

    if(!CopyFileA(entry.sourcePath, destination, FALSE))
    {
        CString error;
        error.Format("Could not copy %s to %s (Windows error %lu).",
                     (const char *)entry.sourcePath, (const char *)destination, GetLastError());
        QuickDeployRecordError(stats, error);
        return FALSE;
    }
    stats.copied++;
    return TRUE;
}

void EngineXWin::OnQuickSetGamePaths()
{
    if(!m_gStatus)
        return;

    CString coreStart = QuickDeployReadFolderProfile(_T("CoreRoot"));
    if(coreStart.IsEmpty())
        coreStart = m_gStatus->GL_workingDirectory;
    CString coreRoot;
    if(!QuickDeployChooseFolder(GetSafeHwnd(), _T("CoreRoot"),
                                "Choose the Core game folder (it must contain GameFiles).",
                                coreStart, TRUE, TRUE, coreRoot))
        return;

    CString serverStart = QuickDeployReadFolderProfile(_T("ServerRoot"));
    if(serverStart.IsEmpty())
    {
        serverStart = "C:";
        serverStart += (char)92;
        serverStart += "ea server";
    }
    CString serverRoot;
    if(!QuickDeployChooseFolder(GetSafeHwnd(), _T("ServerRoot"),
                                "Choose the EA server folder (usually C:\\ea server).",
                                serverStart, FALSE, TRUE, serverRoot))
        return;

    CString patchRoot = QuickDeployResolvePatchFolder(serverRoot);
    CString message;
    message.Format("Send To Game is ready.\n\nCore: %s\nEA server patch folder: %s\n\n"
                   "Use Quick > Send To Game whenever you have saved your changes.",
                   (const char *)coreRoot, (const char *)patchRoot);
    MessageBox(message, "Send To Game Folders", MB_OK | MB_ICONINFORMATION);
}

void EngineXWin::OnQuickSendToGame()
{
    if(!m_gStatus)
        return;

    CString gameFiles = m_gStatus->GL_pathGameFiles;
    if(gameFiles.IsEmpty())
        gameFiles = QuickDeployJoinPath(m_gStatus->GL_workingDirectory, "GameFiles");

    CString entityDb = m_quickEntityDatabasePath;
    if(entityDb.IsEmpty())
        entityDb = m_gStatus->m_loadedEntityDatabasePath;
    if(entityDb.IsEmpty())
        entityDb = QuickDeployJoinPath(gameFiles, "EARevolution.EDB");
    CString itemDb = m_gStatus->m_itemManager ? m_gStatus->m_itemManager->m_filePath : CString();
    if(!itemDb.IsEmpty() && itemDb.Right(4).CompareNoCase(".APX") != 0)
    {
        MessageBox("The loaded item database is not an .APX file. Save it as an APX database before sending it to the game.",
                   "Send To Game", MB_OK | MB_ICONWARNING);
        return;
    }
    if(itemDb.IsEmpty())
        itemDb = QuickDeployJoinPath(gameFiles, "EARevolution.APX");
    CString menuDb = m_quickMenuSystemPath;
    if(menuDb.IsEmpty() && m_gStatus)
        menuDb = m_gStatus->m_loadedMenuSystemPath;
    if(menuDb.IsEmpty())
        menuDb = QuickDeployJoinPath(gameFiles, "EARevolution.MOD");

    CString missing;
    if(!QuickDeployFileExists(entityDb)) missing += "\n  Entity database: " + entityDb;
    if(!QuickDeployFileExists(itemDb)) missing += "\n  Item database: " + itemDb;
    if(!QuickDeployFileExists(menuDb)) missing += "\n  Menu system: " + menuDb;
    if(!missing.IsEmpty())
    {
        MessageBox("Send To Game was not started because these saved files were not found:" + missing +
                   "\n\nSave them under GameFiles first, then try again.",
                   "Send To Game", MB_OK | MB_ICONWARNING);
        return;
    }

    std::vector<QuickDeployFile> files;
    QuickDeployFile edbEntry; edbEntry.sourcePath = entityDb; edbEntry.relativePath = "GameFiles/EARevolution.EDB";
    QuickDeployFile itemEntry; itemEntry.sourcePath = itemDb; itemEntry.relativePath = "GameFiles/EARevolution.APX";
    QuickDeployFile menuEntry; menuEntry.sourcePath = menuDb; menuEntry.relativePath = "GameFiles/EARevolution.MOD";
    files.push_back(edbEntry);
    files.push_back(itemEntry);
    files.push_back(menuEntry);

    CString textureBankNotice;
    CString textureBank = QuickDeployJoinPath(gameFiles, "Static.TXB");
    if(QuickDeployFileExists(textureBank))
    {
        QuickDeployFile textureBankEntry;
        textureBankEntry.sourcePath = textureBank;
        textureBankEntry.relativePath = "GameFiles/Static.TXB";
        files.push_back(textureBankEntry);
    }
    else
    {
        textureBankNotice = "Static.TXB was not found in the source GameFiles folder; texture-bank changes were not copied.";
    }

    CString textureFolder = m_gStatus->GL_pathMapModels;
    if(textureFolder.IsEmpty())
        textureFolder = QuickDeployJoinPath(m_gStatus->GL_workingDirectory, "MapsModels");
    if(!QuickDeployDirectoryExists(textureFolder))
    {
        MessageBox("The MapsModels texture folder could not be found:\n\n" + textureFolder,
                   "Send To Game", MB_OK | MB_ICONWARNING);
        return;
    }
    QuickDeployCollectTextures(textureFolder, "MapsModels", files);

    CString coreRoot, serverRoot;
    CString coreStart = m_gStatus->GL_workingDirectory;
    if(!QuickDeployChooseFolder(GetSafeHwnd(), _T("CoreRoot"),
                                "Choose the Core game folder (it must contain GameFiles).",
                                coreStart, TRUE, FALSE, coreRoot))
        return;
    CString serverStart = "C:";
    serverStart += (char)92;
    serverStart += "ea server";
    if(!QuickDeployChooseFolder(GetSafeHwnd(), _T("ServerRoot"),
                                "Choose the EA server folder (usually C:\\ea server).",
                                serverStart, FALSE, FALSE, serverRoot))
        return;
    CString patchRoot = QuickDeployResolvePatchFolder(serverRoot);

    CString stamp;
    QuickDeployMakeTimestamp(stamp);
    QuickDeployStats coreStats = { 0, 0, 0, 0, CString() };
    QuickDeployStats serverStats = { 0, 0, 0, 0, CString() };
    for(size_t i = 0; i < files.size(); ++i)
    {
        QuickDeployOneFile(files[i], coreRoot, stamp, coreStats);
        QuickDeployOneFile(files[i], patchRoot, stamp, serverStats);
    }

    CString report;
    report.Format("Send To Game finished.\n\nCore: %d copied, %d already current, %d backed up, %d failed.\n"
                  "EA server: %d copied, %d already current, %d backed up, %d failed.\n\n"
                  "Changed destination files are backed up in _EA_Backups/%s.",
                  coreStats.copied, coreStats.unchanged, coreStats.backups, coreStats.failures,
                  serverStats.copied, serverStats.unchanged, serverStats.backups, serverStats.failures,
                  (const char *)stamp);
    if(!coreStats.errors.IsEmpty()) report += "\n\nCore errors:" + coreStats.errors;
    if(!serverStats.errors.IsEmpty()) report += "\n\nEA server errors:" + serverStats.errors;
    if(!textureBankNotice.IsEmpty()) report += "\n\nNOTE: " + textureBankNotice;
    if(coreStats.failures > 8 || serverStats.failures > 8)
        report += "\n\n(Only the first eight errors per destination are listed.)";
    BOOL success = coreStats.failures == 0 && serverStats.failures == 0 && textureBankNotice.IsEmpty();
    MessageBox(report, "Send To Game", MB_OK | (success ? MB_ICONINFORMATION : MB_ICONWARNING));
}

//------------------------------------------------------------------------------------//
// Quick > New Race Wizard
//   Creates a new EDB character from the selected base race, clones that race's item
//   bindings and character-creation page, inserts the page into the existing race
//   navigation cycle, and copies the id=0 / ENDLESS server-start row to AXServer SQL.
//   Each local database is backed up before it is replaced.  The SQL source row and
//   target key are validated before any local data is changed.
//------------------------------------------------------------------------------------//
struct QuickRaceStartScriptRow
{
    CString scriptName;
    CString description;
    CString scriptData;
    CString gameSpecific;
    int scriptBasis;
    int edbSpecific;
    QuickRaceStartScriptRow() : scriptBasis(0), edbSpecific(-1) {}
};

struct QuickRaceAddedItemRef
{
    CAttachmentRefList *list;
    CAttachmentRefObj *ref;
};

struct QuickRaceMenuScriptSwap
{
    CMenuScript **slot;
    CMenuScript *oldScript;
    CMenuScript *newScript;
};

static CString QuickRaceSqlStringLiteral(const CString &value)
{
    CString escaped = value;
    escaped.Replace("'", "''");
    return CString("N'") + escaped + "'";
}

static BOOL QuickRaceQueryInt(CDBConnectionSystem *db, const CString &query, int &value)
{
    value = -1;
    if(!db)
        return FALSE;
    _RecordsetPtr recordset = db->Query(query);
    if(recordset == NULL)
        return FALSE;
    value = db->GetIntData("RowCount", recordset);
    db->Close(recordset);
    return TRUE;
}

static BOOL QuickRaceLoadStartScript(CDBConnectionSystem *db, int sourceRaceID,
                                     QuickRaceStartScriptRow &row, CString &error)
{
    CString countQuery;
    countQuery.Format("SELECT COUNT(*) AS RowCount FROM [tblServerScripts] "
                       "WHERE [id] = 0 AND [gameSpecific] = 'ENDLESS' AND [edbspecific] = %d",
                       sourceRaceID);
    int count = -1;
    if(!QuickRaceQueryInt(db, countQuery, count))
    {
        error = "Could not validate the source start-script row in tblServerScripts.";
        return FALSE;
    }
    if(count != 1)
    {
        error.Format("Expected exactly one id=0, gameSpecific=ENDLESS start row for TRC_%d; found %d. "
                     "No files have been changed.", sourceRaceID, count);
        return FALSE;
    }

    CString selectQuery;
    selectQuery.Format("SELECT [scriptName], [Description], [scriptData], [scriptBasis], "
                       "[gameSpecific], [edbspecific] FROM [tblServerScripts] "
                       "WHERE [id] = 0 AND [gameSpecific] = 'ENDLESS' AND [edbspecific] = %d",
                       sourceRaceID);
    _RecordsetPtr rowset = db->Query(selectQuery);
    if(rowset == NULL)
    {
        error = "The source start-script row could not be read from tblServerScripts.";
        return FALSE;
    }

    row.scriptName = db->GetStringData("scriptName", rowset);
    row.description = db->GetStringData("Description", rowset);
    row.scriptData = db->GetStringData("scriptData", rowset);
    row.scriptBasis = db->GetIntData("scriptBasis", rowset);
    row.gameSpecific = db->GetStringData("gameSpecific", rowset);
    row.edbSpecific = db->GetIntData("edbspecific", rowset);
    db->Close(rowset);

    if(row.scriptData.IsEmpty())
    {
        error.Format("The validated start-script row for TRC_%d has empty scriptData. "
                     "No files have been changed.", sourceRaceID);
        return FALSE;
    }
    if(row.gameSpecific.CompareNoCase("ENDLESS") != 0 || row.edbSpecific != sourceRaceID)
    {
        error.Format("The source row did not match TRC_%d / ENDLESS when read back. "
                     "No files have been changed.", sourceRaceID);
        return FALSE;
    }
    return TRUE;
}

static BOOL QuickRaceTargetStartScriptIsFree(CDBConnectionSystem *db, int raceID,
                                             BOOL &isFree, CString &error)
{
    isFree = FALSE;
    CString query;
    query.Format("SELECT COUNT(*) AS RowCount FROM [tblServerScripts] "
                 "WHERE [id] = 0 AND [gameSpecific] = 'ENDLESS' AND [edbspecific] = %d",
                 raceID);
    int count = -1;
    if(!QuickRaceQueryInt(db, query, count))
    {
        error = "Could not check whether the new race already has an AXServer start-script row.";
        return FALSE;
    }
    if(count != 0)
    {
        error.Format("tblServerScripts already contains %d id=0 / ENDLESS start row(s) for TRC_%d. "
                     "No files have been changed.", count, raceID);
        return FALSE;
    }
    isFree = TRUE;
    return TRUE;
}

static BOOL QuickRaceReadFunctionArgument(const CString &text, const char *functionName,
                                          int argumentIndex, int &value)
{
    if(!functionName || argumentIndex < 0)
        return FALSE;
    CString signature = functionName;
    signature += "(";

    int matchCount = 0;
    int parsedValue = 0;
    int searchFrom = 0;
    int found = -1;
    while((found = text.Find(signature, searchFrom)) >= 0)
    {
        if(found > 0)
        {
            unsigned char previous = (unsigned char)text.GetAt(found - 1);
            if(isalnum(previous) || previous == '_')
            {
                searchFrom = found + signature.GetLength();
                continue;
            }
        }

        int open = found + signature.GetLength() - 1;
        int close = text.Find(')', open + 1);
        if(close < 0)
            return FALSE;
        int argStart = open + 1;
        for(int arg = 0; arg < argumentIndex; ++arg)
        {
            int comma = text.Find(',', argStart);
            if(comma < 0 || comma > close)
                return FALSE;
            argStart = comma + 1;
        }
        int argEnd = text.Find(',', argStart);
        if(argEnd < 0 || argEnd > close)
            argEnd = close;

        CString argument = text.Mid(argStart, argEnd - argStart);
        argument.Trim();
        if(argument.IsEmpty())
            return FALSE;
        char *end = NULL;
        long number = strtol((const char *)argument, &end, 10);
        if(!end || end == (const char *)argument)
            return FALSE;
        while(*end && isspace((unsigned char)*end))
            ++end;
        if(*end != '\0' || number < INT_MIN || number > INT_MAX)
            return FALSE;

        parsedValue = (int)number;
        ++matchCount;
        searchFrom = close + 1;
    }

    if(matchCount != 1)
        return FALSE;
    value = parsedValue;
    return TRUE;
}

static BOOL QuickRaceRewriteFunctionArgument(CString &text, const char *functionName,
                                            int argumentIndex, int newValue)
{
    int oldValue = 0;
    if(!QuickRaceReadFunctionArgument(text, functionName, argumentIndex, oldValue))
        return FALSE;
    (void)oldValue;

    CString signature = functionName;
    signature += "(";
    int found = text.Find(signature);
    while(found >= 0 && found > 0)
    {
        unsigned char previous = (unsigned char)text.GetAt(found - 1);
        if(!isalnum(previous) && previous != '_')
            break;
        found = text.Find(signature, found + signature.GetLength());
    }
    if(found < 0)
        return FALSE;

    int open = found + signature.GetLength() - 1;
    int close = text.Find(')', open + 1);
    if(close < 0)
        return FALSE;
    int argStart = open + 1;
    for(int arg = 0; arg < argumentIndex; ++arg)
    {
        int comma = text.Find(',', argStart);
        if(comma < 0 || comma > close)
            return FALSE;
        argStart = comma + 1;
    }
    int argEnd = text.Find(',', argStart);
    if(argEnd < 0 || argEnd > close)
        argEnd = close;

    CString oldArgument = text.Mid(argStart, argEnd - argStart);
    int leading = 0;
    while(leading < oldArgument.GetLength() && isspace((unsigned char)oldArgument.GetAt(leading)))
        ++leading;
    int trailing = oldArgument.GetLength();
    while(trailing > leading && isspace((unsigned char)oldArgument.GetAt(trailing - 1)))
        --trailing;
    CString replacement;
    replacement.Format("%d", newValue);
    CString newArgument = oldArgument.Left(leading) + replacement + oldArgument.Mid(trailing);
    text = text.Left(argStart) + newArgument + text.Mid(argEnd);
    return TRUE;
}

static CMenuScript *QuickRaceBuildScript(const CString &text, const CMenuScript *source)
{
    CMenuScript *script = new CMenuScript();
    if(source)
    {
        script->m_callingMenu = source->m_callingMenu;
        script->m_callingVisual = source->m_callingVisual;
        script->m_tempRegString = source->m_tempRegString;
        script->m_paramConversion = source->m_paramConversion;
        script->m_waitOnInputFlag = source->m_waitOnInputFlag;
        script->m_fadeRate = source->m_fadeRate;
        script->m_timeStamp = source->m_timeStamp;
    }

    int length = text.GetLength();
    int start = 0;
    while(start < length)
    {
        int newline = text.Find('\n', start);
        int end = (newline < 0) ? length : newline;
        CString line = text.Mid(start, end - start);
        int leading = 0;
        while(leading < line.GetLength() &&
              (line.GetAt(leading) == ' ' || line.GetAt(leading) == '\t'))
            ++leading;
        if(leading > 0)
            line = line.Mid(leading);
        if(line.GetLength() > 0 && line.GetAt(line.GetLength() - 1) == '\r')
            line = line.Left(line.GetLength() - 1);

        if(!line.IsEmpty())
        {
            if(line.Find('(') <= 0 || line.Find(')') < 0)
            {
                delete script;
                return NULL;
            }
            int lineLength = line.GetLength();
            std::vector<char> buffer((size_t)lineLength + 2, 0);
            memcpy(&buffer[0], (const char *)line, lineLength);
            buffer[lineLength] = '\n';
            buffer[lineLength + 1] = '\0';
            script->AddLine(&buffer[0]);
        }
        else if(newline >= 0)
            script->m_scriptText += "\n";

        if(newline < 0)
            break;
        start = newline + 1;
    }

    if(!script->m_pFirstLine)
    {
        delete script;
        return NULL;
    }
    // CMenuScript::AddLine leaves unknown commands at the CScriptLine default
    // function value (-1) without returning an error. Reject those scripts here.
    for(CScriptLine *line = script->m_pFirstLine; line; line = line->m_pNextLine)
    {
        if(line->m_function < 0)
        {
            delete script;
            return NULL;
        }
    }
    return script;
}

static BOOL QuickRaceReplaceScriptArgument(CMenuScript **slot, const char *functionName,
                                           int argumentIndex, int newValue,
                                           std::vector<QuickRaceMenuScriptSwap> *swaps)
{
    if(!slot || !*slot)
        return FALSE;
    CMenuScript *oldScript = *slot;
    CString text = oldScript->m_scriptText;
    if(!QuickRaceRewriteFunctionArgument(text, functionName, argumentIndex, newValue))
        return FALSE;

    CMenuScript *newScript = QuickRaceBuildScript(text, oldScript);
    if(!newScript)
        return FALSE;
    *slot = newScript;
    if(swaps)
    {
        QuickRaceMenuScriptSwap swap;
        swap.slot = slot;
        swap.oldScript = oldScript;
        swap.newScript = newScript;
        swaps->push_back(swap);
    }
    else
        delete oldScript;
    return TRUE;
}

static void QuickRaceRollbackMenuScriptSwaps(std::vector<QuickRaceMenuScriptSwap> &swaps)
{
    for(size_t i = swaps.size(); i > 0; --i)
    {
        QuickRaceMenuScriptSwap &swap = swaps[i - 1];
        if(swap.slot && *swap.slot == swap.newScript)
        {
            delete *swap.slot;
            *swap.slot = swap.oldScript;
        }
    }
    swaps.clear();
}

static void QuickRaceCommitMenuScriptSwaps(std::vector<QuickRaceMenuScriptSwap> &swaps)
{
    for(size_t i = 0; i < swaps.size(); ++i)
        delete swaps[i].oldScript;
    swaps.clear();
}

static CMenuWindow *QuickRaceFindMenuByID(CMenuVisualList *database, int menuID)
{
    if(!database)
        return NULL;
    for(POSITION pos = database->GetHeadPosition(); pos != NULL;)
    {
        CMenuVisual *visual = (CMenuVisual *)database->GetNext(pos);
        if(visual && visual->GetType() == MENU_VISTYPE_WINDOW && visual->m_visualID == menuID)
            return (CMenuWindow *)visual;
    }
    return NULL;
}

static CMenuButton *QuickRaceFindButton(CMenuWindow *menu, const char *name)
{
    if(!menu || !menu->m_childList)
        return NULL;
    return dynamic_cast<CMenuButton *>(menu->m_childList->GetVisualByName(name, false));
}

static BOOL QuickRaceReadMenuRace(CMenuWindow *menu, int &entityVisualID, int &raceID)
{
    if(!menu || !menu->m_loadScript)
        return FALSE;
    int referencedMenuID = 0;
    if(!QuickRaceReadFunctionArgument(menu->m_loadScript->m_scriptText,
                                      "InitEntityFromDBID", 0, referencedMenuID) ||
       referencedMenuID != -1 ||
       !QuickRaceReadFunctionArgument(menu->m_loadScript->m_scriptText,
                                      "InitEntityFromDBID", 1, entityVisualID) ||
       !QuickRaceReadFunctionArgument(menu->m_loadScript->m_scriptText,
                                      "InitEntityFromDBID", 2, raceID))
        return FALSE;
    return TRUE;
}

static BOOL QuickRaceFindCreationMenu(CMenuVisualList *database, int raceID,
                                      CMenuWindow **sourceMenu, int &oldNextMenuID,
                                      CMenuWindow **oldNextMenu, CString &error)
{
    *sourceMenu = NULL;
    *oldNextMenu = NULL;
    oldNextMenuID = -1;
    if(!database || database->GetCount() == 0)
    {
        error = "No menu system is loaded.";
        return FALSE;
    }

    CMenuWindow *characterSelect = dynamic_cast<CMenuWindow *>(
        database->GetVisualByName("CharacterSelect", false));
    CMenuButton *createButton = QuickRaceFindButton(characterSelect, "CreateButton");
    int startMenuID = -1;
    if(!createButton || !createButton->m_clickScript ||
       !QuickRaceReadFunctionArgument(createButton->m_clickScript->m_scriptText,
                                      "OpenMenu", 0, startMenuID))
    {
        error = "The loaded menu system has no readable CharacterSelect > CreateButton link.";
        return FALSE;
    }

    int currentMenuID = startMenuID;
    BOOL closedCycle = FALSE;
    std::vector<int> visited;
    CMenuWindow *matchedSource = NULL;
    CMenuWindow *matchedNextMenu = NULL;
    int matchedNextID = -1;
    int maxSteps = database->GetCount() + 1;
    for(int step = 0; step < maxSteps; ++step)
    {
        if(step > 0 && currentMenuID == startMenuID)
        {
            closedCycle = TRUE;
            break;
        }
        for(size_t i = 0; i < visited.size(); ++i)
        {
            if(visited[i] == currentMenuID)
            {
                error = "The character-creation race navigation loops before returning to its starting race.";
                return FALSE;
            }
        }
        visited.push_back(currentMenuID);

        CMenuWindow *current = QuickRaceFindMenuByID(database, currentMenuID);
        CMenuButton *nextButton = QuickRaceFindButton(current, "ModelNextButton");
        if(!current || !nextButton || !nextButton->m_clickScript)
        {
            error = "A race page in the CharacterSelect navigation cycle is missing ModelNextButton.";
            return FALSE;
        }
        int nextID = -1;
        int closeID = -1;
        if(!QuickRaceReadFunctionArgument(nextButton->m_clickScript->m_scriptText,
                                          "OpenMenu", 0, nextID) ||
           !QuickRaceReadFunctionArgument(nextButton->m_clickScript->m_scriptText,
                                          "CloseMenu", 0, closeID) ||
           closeID != currentMenuID || nextID == currentMenuID)
        {
            error = "A ModelNextButton link in the character-creation cycle is invalid.";
            return FALSE;
        }
        CMenuWindow *next = QuickRaceFindMenuByID(database, nextID);
        CMenuButton *nextPreviousButton = QuickRaceFindButton(next, "ModelPrevButton");
        int reverseID = -1;
        int reverseCloseID = -1;
        if(!next || !nextPreviousButton || !nextPreviousButton->m_clickScript ||
           !QuickRaceReadFunctionArgument(nextPreviousButton->m_clickScript->m_scriptText,
                                          "OpenMenu", 0, reverseID) ||
           !QuickRaceReadFunctionArgument(nextPreviousButton->m_clickScript->m_scriptText,
                                          "CloseMenu", 0, reverseCloseID) ||
           reverseID != currentMenuID || reverseCloseID != nextID)
        {
            error = "The ModelNextButton / ModelPrevButton links in the race cycle do not agree.";
            return FALSE;
        }

        int entityVisualID = -1;
        int menuRaceID = -1;
        if(QuickRaceReadMenuRace(current, entityVisualID, menuRaceID) && menuRaceID == raceID)
        {
            if(matchedSource)
            {
                error.Format("More than one character-creation page in the current cycle uses TRC_%d. "
                             "Resolve the duplicate before creating a race.", raceID);
                return FALSE;
            }
            matchedSource = current;
            matchedNextMenu = next;
            matchedNextID = nextID;
        }
        currentMenuID = nextID;
    }

    if(!closedCycle)
    {
        error = "The character-creation race navigation did not return to its starting page.";
        return FALSE;
    }
    if(!matchedSource || !matchedNextMenu)
    {
        error.Format("TRC_%d has no page in the character-creation cycle opened by CharacterSelect. "
                     "Load a menu system that includes this race before using the wizard.", raceID);
        return FALSE;
    }
    *sourceMenu = matchedSource;
    *oldNextMenu = matchedNextMenu;
    oldNextMenuID = matchedNextID;
    return TRUE;
}

static BOOL QuickRaceMenuUsesRaceID(CMenuVisualList *database, int raceID)
{
    if(!database)
        return FALSE;
    for(POSITION pos = database->GetHeadPosition(); pos != NULL;)
    {
        CMenuVisual *visual = (CMenuVisual *)database->GetNext(pos);
        if(!visual || visual->GetType() != MENU_VISTYPE_WINDOW)
            continue;
        int entityVisualID = -1;
        int menuRaceID = -1;
        if(QuickRaceReadMenuRace((CMenuWindow *)visual, entityVisualID, menuRaceID) &&
           menuRaceID == raceID)
            return TRUE;
    }
    return FALSE;
}

static int QuickRaceCountItemBindings(CItemManager *items, int raceID)
{
    int count = 0;
    if(!items || !items->m_listArray || items->m_indexedVarCount == 0)
        return 0;
    for(unsigned int cell = 0; cell < items->m_cellCount; ++cell)
    {
        for(CAxLinkObj *pos = items->m_listArray[0].m_axList[cell].GetHeadPosition_unsafe(); pos != NULL;)
        {
            CItemObj *item = (CItemObj *)items->m_listArray[0].m_axList[cell].GetNext_unsafe(&pos);
            if(!item || !item->m_attachmentRefList)
                continue;
            for(POSITION refPos = item->m_attachmentRefList->GetHeadPosition(); refPos != NULL;)
            {
                CAttachmentRefObj *ref = (CAttachmentRefObj *)item->m_attachmentRefList->GetNext(refPos);
                if(ref && ref->m_raceBind == raceID)
                    ++count;
            }
        }
    }
    return count;
}

static BOOL QuickRaceCopyItemBindings(CItemManager *items, int sourceRaceID, int newRaceID,
                                      std::vector<QuickRaceAddedItemRef> &addedRefs,
                                      int &itemsCopied, int &bindingsCopied)
{
    itemsCopied = 0;
    bindingsCopied = 0;
    if(!items || !items->m_listArray || items->m_indexedVarCount == 0)
        return FALSE;

    for(unsigned int cell = 0; cell < items->m_cellCount; ++cell)
    {
        for(CAxLinkObj *pos = items->m_listArray[0].m_axList[cell].GetHeadPosition_unsafe(); pos != NULL;)
        {
            CItemObj *item = (CItemObj *)items->m_listArray[0].m_axList[cell].GetNext_unsafe(&pos);
            if(!item || !item->m_attachmentRefList)
                continue;

            std::vector<CAttachmentRefObj *> sourceRefs;
            for(POSITION refPos = item->m_attachmentRefList->GetHeadPosition(); refPos != NULL;)
            {
                CAttachmentRefObj *ref = (CAttachmentRefObj *)item->m_attachmentRefList->GetNext(refPos);
                if(ref && ref->m_raceBind == sourceRaceID)
                    sourceRefs.push_back(ref);
            }
            if(sourceRefs.empty())
                continue;

            for(size_t i = 0; i < sourceRefs.size(); ++i)
            {
                CAttachmentRefObj *copy = new CAttachmentRefObj();
                copy->m_attachmentType = sourceRefs[i]->m_attachmentType;
                copy->m_raceBind = newRaceID;
                copy->m_dimIndex = sourceRefs[i]->m_dimIndex;
                copy->m_dimSwitchTo = sourceRefs[i]->m_dimSwitchTo;
                copy->m_fireQueOvrd = sourceRefs[i]->m_fireQueOvrd;
                item->m_attachmentRefList->AddTail(copy);
                QuickRaceAddedItemRef added;
                added.list = item->m_attachmentRefList;
                added.ref = copy;
                addedRefs.push_back(added);
                ++bindingsCopied;
            }
            ++itemsCopied;
        }
    }
    return itemsCopied > 0 && bindingsCopied > 0;
}

static void QuickRaceRollbackItemBindings(std::vector<QuickRaceAddedItemRef> &addedRefs)
{
    for(size_t i = addedRefs.size(); i > 0; --i)
    {
        CAttachmentRefList *list = addedRefs[i - 1].list;
        CAttachmentRefObj *ref = addedRefs[i - 1].ref;
        if(!list || !ref)
            continue;
        for(POSITION pos = list->GetHeadPosition(); pos != NULL;)
        {
            POSITION current = pos;
            CAttachmentRefObj *candidate = (CAttachmentRefObj *)list->GetNext(pos);
            if(candidate == ref)
            {
                list->RemoveAt(current);
                delete ref;
                break;
            }
        }
    }
    addedRefs.clear();
}

static CMovementObj *QuickRaceCloneCharacter(CMovementObj *source, const CString &newName,
                                             int newRaceID, GlobalStatusVariables *gs)
{
    if(!source || !gs)
        return NULL;
    CMovementObj *copy = NULL;
    try
    {
        CMemFile memory;
        {
            CArchive output(&memory, CArchive::store);
            output << source;
            output.Close();
        }
        memory.SeekToBegin();
        {
            CArchive input(&memory, CArchive::load);
            input >> copy;
            input.Close();
        }
    }
    catch(CException *exceptionPtr)
    {
        exceptionPtr->Delete();
        if(copy)
            delete copy;
        return NULL;
    }
    catch(...)
    {
        if(copy)
            delete copy;
        return NULL;
    }
    if(!copy || !copy->List_skeletalObject)
    {
        if(copy)
            delete copy;
        return NULL;
    }

    copy->List_name = newName;
    copy->List_traceNumber = gs->GlobalTraceNumber++;
    copy->m_uniqueId = newRaceID;
    copy->List_skeletalObject->m_SkeletalName = newName;
    return copy;
}

static CString QuickRaceUniqueMenuName(CMenuVisualList *database, const CString &raceName)
{
    CString base = raceName;
    base.Replace(" ", "");
    base.Replace("'", "");
    base.Replace("-", "");
    if(base.IsEmpty())
        base = "NewRace";
    base += "CharacterCreate";

    CString candidate = base;
    int suffix = 2;
    while(database && database->GetVisualByName(candidate, false))
    {
        candidate.Format("%s_%d", (const char *)base, suffix);
        ++suffix;
    }
    return candidate;
}

static BOOL QuickRaceBuildMenuClone(CMenuWindow *sourceMenu, int sourceMenuID,
                                    int oldNextMenuID, int newMenuID, int newRaceID,
                                    const CString &raceName, LPDIRECT3DDEVICE9 device,
                                    CMenuVisualList *database, CMenuWindow **newMenu,
                                    CString &error)
{
    *newMenu = NULL;
    if(!sourceMenu || !sourceMenu->m_childList || !sourceMenu->m_loadScript)
    {
        error = "The selected race's creation page cannot be cloned.";
        return FALSE;
    }

    CMenuVisual *cloneVisual = NULL;
    sourceMenu->Clone(&cloneVisual, device);
    CMenuWindow *clone = dynamic_cast<CMenuWindow *>(cloneVisual);
    if(!clone)
    {
        if(cloneVisual)
            delete cloneVisual;
        error = "The selected race's creation page did not clone as a menu window.";
        return FALSE;
    }

    clone->m_visualID = newMenuID;
    clone->m_name = QuickRaceUniqueMenuName(database, raceName);
    clone->m_instance = FALSE;

    int entityVisualID = -1;
    int sourceRaceID = -1;
    if(!QuickRaceReadMenuRace(sourceMenu, entityVisualID, sourceRaceID) ||
       sourceRaceID <= 0 || sourceRaceID == newRaceID || sourceMenu->m_visualID != sourceMenuID)
    {
        delete clone;
        error = "The selected race's page does not have a valid InitEntityFromDBID load script.";
        return FALSE;
    }
    CMenuEntity *menuEntity = dynamic_cast<CMenuEntity *>(
        clone->m_childList->GetVisualByID(entityVisualID, false));
    CMenuText *raceNameText = dynamic_cast<CMenuText *>(
        clone->m_childList->GetVisualByName("RaceName", false));
    CMenuButton *prevButton = QuickRaceFindButton(clone, "ModelPrevButton");
    CMenuButton *nextButton = QuickRaceFindButton(clone, "ModelNextButton");
    CMenuButton *cancelButton = QuickRaceFindButton(clone, "CancelButton");
    CMenuButton *createButton = QuickRaceFindButton(clone, "CreateButton");
    int createEntityVisualID = -1;
    int nameTextBoxID = -1;
    int spawnPosition = -1;
    if(!menuEntity || !raceNameText || !prevButton || !prevButton->m_clickScript ||
       !nextButton || !nextButton->m_clickScript || !cancelButton || !cancelButton->m_clickScript ||
       !createButton || !createButton->m_clickScript ||
       !QuickRaceReadFunctionArgument(createButton->m_clickScript->m_scriptText,
                                      "CreateCharacter", 0, createEntityVisualID) ||
       !QuickRaceReadFunctionArgument(createButton->m_clickScript->m_scriptText,
                                      "CreateCharacter", 1, nameTextBoxID) ||
       !QuickRaceReadFunctionArgument(createButton->m_clickScript->m_scriptText,
                                      "CreateCharacter", 2, spawnPosition) ||
       spawnPosition < 0 || createEntityVisualID != entityVisualID ||
       !clone->m_childList->GetVisualByID(nameTextBoxID, false))
    {
        delete clone;
        error = "The selected race's creation page is missing a valid entity, name box, or navigation button.";
        return FALSE;
    }

    menuEntity->m_entityID = newRaceID;
    raceNameText->SetText((const char *)raceName);
    if(!QuickRaceReplaceScriptArgument(&clone->m_loadScript,
                                      "InitEntityFromDBID", 2, newRaceID, NULL) ||
       !QuickRaceReplaceScriptArgument(&prevButton->m_clickScript,
                                      "OpenMenu", 0, sourceMenuID, NULL) ||
       !QuickRaceReplaceScriptArgument(&prevButton->m_clickScript,
                                      "CloseMenu", 0, newMenuID, NULL) ||
       !QuickRaceReplaceScriptArgument(&nextButton->m_clickScript,
                                      "OpenMenu", 0, oldNextMenuID, NULL) ||
       !QuickRaceReplaceScriptArgument(&nextButton->m_clickScript,
                                      "CloseMenu", 0, newMenuID, NULL) ||
       !QuickRaceReplaceScriptArgument(&cancelButton->m_clickScript,
                                      "CloseMenu", 0, newMenuID, NULL))
    {
        delete clone;
        error = "The selected race's creation scripts could not be safely retargeted.";
        return FALSE;
    }

    *newMenu = clone;
    return TRUE;
}

static BOOL QuickRaceValidateName(CString &name, CString &error)
{
    name.Trim();
    if(name.IsEmpty())
    {
        error = "Enter a name for the new race.";
        return FALSE;
    }
    if(name.GetLength() > 48)
    {
        error = "Use a race name of 48 characters or fewer so it fits the creation screen.";
        return FALSE;
    }
    for(int i = 0; i < name.GetLength(); ++i)
    {
        unsigned char ch = (unsigned char)name.GetAt(i);
        if(ch < 32 || ch > 126 || ch == '<' || ch == '>' || ch == '&')
        {
            error = "Use printable basic-ASCII characters in the race name; <, >, and & are not supported in menu text.";
            return FALSE;
        }
    }
    return TRUE;
}

static BOOL QuickRaceSaveEntityDatabase(GlobalStatusVariables *gs, const CString &path,
                                        CString &error)
{
    CFile file;
    CFileException exception;
    if(!file.Open(path, CFile::modeCreate | CFile::modeWrite, &exception))
    {
        char text[512] = { 0 };
        exception.GetErrorMessage(text, sizeof(text));
        error.Format("Could not open the EDB for writing:\n%s\n\n%s",
                     (const char *)path, text);
        return FALSE;
    }

    BOOL saved = FALSE;
    try
    {
        CArchive archive(&file, CArchive::store);
        archive << gs->m_entObjList;
        archive.Close();
        file.Close();
        saved = TRUE;
    }
    catch(CException *exceptionPtr)
    {
        char text[512] = { 0 };
        exceptionPtr->GetErrorMessage(text, sizeof(text));
        error = text;
        exceptionPtr->Delete();
        file.Abort();
    }
    catch(...)
    {
        error = "An unexpected error occurred while writing the EDB.";
        file.Abort();
    }
    return saved;
}

static BOOL QuickRaceSaveItemDatabase(CItemManager *items, const CString &path,
                                      CString &error)
{
    if(!items)
    {
        error = "The item database is not loaded.";
        return FALSE;
    }
    CString writablePath = path;
    char *fileName = writablePath.GetBuffer();
    BOOL saved = FALSE;
    try
    {
        saved = items->Save(fileName);
    }
    catch(CException *exceptionPtr)
    {
        char text[512] = { 0 };
        exceptionPtr->GetErrorMessage(text, sizeof(text));
        error = text;
        exceptionPtr->Delete();
    }
    catch(...)
    {
        error = "An unexpected error occurred while writing the APX.";
    }
    writablePath.ReleaseBuffer();
    if(!saved && error.IsEmpty())
        error.Format("The APX save failed:\n%s", (const char *)path);
    return saved;
}

static BOOL QuickRaceSaveMenuSystem(GlobalStatusVariables *gs, const CString &path,
                                    CString &error)
{
    if(!gs || !gs->m_menuFontList || !gs->m_menuIconDatabase || !gs->m_menuDatabase ||
       !gs->m_textureDataBase || !gs->m_3dDevice)
    {
        error = "The loaded menu system is incomplete and cannot be saved.";
        return FALSE;
    }
    for(POSITION pos = gs->m_menuDatabase->GetHeadPosition(); pos != NULL;)
    {
        CMenuVisual *visual = (CMenuVisual *)gs->m_menuDatabase->GetNext(pos);
        CMenuWindow *menu = dynamic_cast<CMenuWindow *>(visual);
        if(menu)
            menu->ReinitMenuTextures(gs->m_textureDataBase, gs->m_menuFontList,
                                     gs->m_3dDevice, gs->m_draggingItem);
    }
    for(int i = 0; i < CURSOR_MAX; ++i)
    {
        if(!gs->m_mouseCursors[i])
        {
            error = "A menu cursor is missing; the menu system was not saved.";
            return FALSE;
        }
    }

    CXMLFileIO xmlOut;
    CString writablePath = path;
    char *fileName = writablePath.GetBuffer();
    bool opened = xmlOut.Open(fileName, true);
    writablePath.ReleaseBuffer();
    if(!opened)
    {
        error.Format("Could not open the menu system for writing:\n%s", (const char *)path);
        return FALSE;
    }

    BOOL saved = FALSE;
    try
    {
        xmlOut.WriteAttribute("FileVersion", "1.0");
        xmlOut.WriteOpeningTag("FontList");
        gs->m_menuFontList->Serialize(xmlOut);
        xmlOut.WriteClosingTag("FontList");
        xmlOut.WriteOpeningTag("IconDatabase");
        gs->m_menuIconDatabase->Serialize(xmlOut);
        xmlOut.WriteClosingTag("IconDatabase");
        for(int i = 0; i < CURSOR_MAX; ++i)
        {
            xmlOut.WriteOpeningTag("MouseCursor");
            gs->m_mouseCursors[i]->Serialize(xmlOut);
            xmlOut.WriteClosingTag("MouseCursor");
        }
        xmlOut.WriteOpeningTag("MenuDatabase");
        gs->m_menuDatabase->Serialize(xmlOut);
        xmlOut.WriteClosingTag("MenuDatabase");
        xmlOut.Close();
        saved = TRUE;
    }
    catch(CException *exceptionPtr)
    {
        char text[512] = { 0 };
        exceptionPtr->GetErrorMessage(text, sizeof(text));
        error = text;
        exceptionPtr->Delete();
        xmlOut.Close();
    }
    catch(...)
    {
        error = "An unexpected error occurred while writing the MOD.";
        xmlOut.Close();
    }
    if(!saved && error.IsEmpty())
        error.Format("The MOD save failed:\n%s", (const char *)path);
    return saved;
}

static BOOL QuickRaceRestoreFile(const CString &path, const CString &backupPath, CString &error)
{
    if(backupPath.IsEmpty())
    {
        if(DeleteFileA(path) || GetLastError() == ERROR_FILE_NOT_FOUND)
            return TRUE;
        error.Format("Could not remove the new file %s (Windows error %lu).",
                     (const char *)path, GetLastError());
        return FALSE;
    }
    if(CopyFileA(backupPath, path, FALSE))
        return TRUE;
    error.Format("Could not restore %s from %s (Windows error %lu).",
                 (const char *)path, (const char *)backupPath, GetLastError());
    return FALSE;
}

static void QuickRaceRollbackInMemory(GlobalStatusVariables *gs,
                                      std::vector<QuickRaceAddedItemRef> &addedRefs,
                                      CMovementObj *newEntity, BOOL entityAdded,
                                      CMenuWindow *newMenu, BOOL menuAdded,
                                      std::vector<QuickRaceMenuScriptSwap> &scriptSwaps)
{
    QuickRaceRollbackMenuScriptSwaps(scriptSwaps);
    QuickRaceRollbackItemBindings(addedRefs);
    if(newMenu)
    {
        if(menuAdded && gs && gs->m_menuDatabase)
        {
            POSITION pos = gs->m_menuDatabase->Find(newMenu);
            if(pos)
                gs->m_menuDatabase->RemoveAt(pos);
        }
        delete newMenu;
    }
    if(newEntity)
    {
        if(entityAdded && gs && gs->m_entObjList)
        {
            POSITION pos = gs->m_entObjList->Find(newEntity);
            if(pos)
                gs->m_entObjList->RemoveAt(pos);
        }
        delete newEntity;
    }
}

void EngineXWin::OnQuickNewRaceWizard()
{
    if(!m_gStatus || !m_gStatus->m_entObjList || !m_gStatus->m_menuDatabase)
    {
        MessageBox("The entity and menu databases must be loaded before creating a race.",
                   "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    HWND treeHwnd = NULL;
    m_entityDlgBar.GetDlgItem(IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM selected = treeHwnd ? TreeView_GetSelection(treeHwnd) : NULL;
    CMovementObj *source = NULL;
    if(selected)
    {
        CString pick = GetTreeItemText(IDC_TREE_ENTITIES, &m_entityDlgBar, selected, 50);
        pick.Replace("TRC_", "");
        source = m_gStatus->m_entObjList->GetByID(atoi((const char *)pick));
    }
    if(!source || !source->List_skeletalObject)
    {
        MessageBox("Pick the male/base character in the Entity panel's Database list first, "
                   "then use Quick > New Race Wizard again.",
                   "New Race Wizard", MB_OK | MB_ICONINFORMATION);
        return;
    }
    if(m_gStatus->m_entObjList->m_lodScriptHasBeenExecuted == 1)
    {
        MessageBox("The EDB cannot be safely saved after LOD processing. Reload the EDB before creating a race.",
                   "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    CItemManager *items = m_gStatus->m_itemManager;
    if(!items || !items->m_listArray || items->m_indexedVarCount == 0)
    {
        MessageBox("No item database is loaded. Load EARevolution.APX in Item Manager first.",
                   "New Race Wizard", MB_OK | MB_ICONINFORMATION);
        return;
    }
    if(QuickRaceCountItemBindings(items, source->m_uniqueId) == 0)
    {
        CString message;
        message.Format("No item bindings were found for TRC_%d in the loaded APX. "
                       "The wizard will not create a race with an empty item set.", source->m_uniqueId);
        MessageBox(message, "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    CString newName = source->List_name + " Female";
    if(!QuickPrompt(GetSafeHwnd(), "New Race Wizard", "Name for the new race:", newName))
        return;
    CString validationError;
    if(!QuickRaceValidateName(newName, validationError))
    {
        MessageBox(validationError, "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }
    if(m_gStatus->m_entObjList->GetByName(newName))
    {
        MessageBox("An entity with that name already exists. Choose a different race name.",
                   "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    CMenuWindow *sourceMenu = NULL;
    CMenuWindow *oldNextMenu = NULL;
    int oldNextMenuID = -1;
    CString menuError;
    if(!QuickRaceFindCreationMenu(m_gStatus->m_menuDatabase, source->m_uniqueId,
                                  &sourceMenu, oldNextMenuID, &oldNextMenu, menuError))
    {
        MessageBox(menuError, "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }
    CMenuButton *sourceNextButton = QuickRaceFindButton(sourceMenu, "ModelNextButton");
    CMenuButton *oldNextPreviousButton = QuickRaceFindButton(oldNextMenu, "ModelPrevButton");
    int oldNextReverseID = -1;
    int oldNextCloseID = -1;
    if(!sourceNextButton || !sourceNextButton->m_clickScript ||
       !oldNextPreviousButton || !oldNextPreviousButton->m_clickScript ||
       !QuickRaceReadFunctionArgument(oldNextPreviousButton->m_clickScript->m_scriptText,
                                      "OpenMenu", 0, oldNextReverseID) ||
       !QuickRaceReadFunctionArgument(oldNextPreviousButton->m_clickScript->m_scriptText,
                                      "CloseMenu", 0, oldNextCloseID) ||
       oldNextReverseID != sourceMenu->m_visualID || oldNextCloseID != oldNextMenuID)
    {
        MessageBox("The race-cycle navigation scripts are incomplete or inconsistent. "
                   "No files have been changed.",
                   "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    int newRaceID = m_gStatus->m_entObjList->GetNextHighID();
    if(newRaceID <= 0 || newRaceID == INT_MAX)
    {
        MessageBox("Could not allocate a safe TRC number for the new race.",
                   "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }
    while(m_gStatus->m_entObjList->GetByID(newRaceID) ||
          QuickRaceCountItemBindings(items, newRaceID) > 0 ||
          QuickRaceMenuUsesRaceID(m_gStatus->m_menuDatabase, newRaceID))
    {
        if(newRaceID == INT_MAX - 1)
        {
            MessageBox("No unused TRC number remains for the new race.",
                       "New Race Wizard", MB_OK | MB_ICONWARNING);
            return;
        }
        ++newRaceID;
    }

    int newMenuID = 42000;
    while(newMenuID < INT_MAX && m_gStatus->m_menuDatabase->GetVisualIDCount(newMenuID) > 0)
        ++newMenuID;
    if(newMenuID >= INT_MAX)
    {
        MessageBox("Could not allocate a menu ID for the new race page.",
                   "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    CString gameFiles = m_gStatus->GL_pathGameFiles;
    if(gameFiles.IsEmpty())
    {
        CString workingDirectory = m_gStatus->GL_workingDirectory;
        if(workingDirectory.IsEmpty())
        {
            char currentDirectory[_MAX_PATH] = { 0 };
            _getcwd(currentDirectory, _MAX_PATH);
            workingDirectory = currentDirectory;
        }
        gameFiles = QuickDeployJoinPath(workingDirectory, "GameFiles");
    }
    CString entityPath = m_quickEntityDatabasePath;
    if(entityPath.IsEmpty())
        entityPath = m_gStatus->m_loadedEntityDatabasePath;
    if(entityPath.IsEmpty())
        entityPath = QuickDeployJoinPath(gameFiles, "EARevolution.EDB");
    CString itemPath = items->m_filePath;
    if(!itemPath.IsEmpty() && itemPath.Right(4).CompareNoCase(".APX") != 0)
    {
        MessageBox("The loaded item database is not an .APX file. Save it as an APX database and load that APX before creating a race.",
                   "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }
    if(itemPath.IsEmpty())
        itemPath = QuickDeployJoinPath(gameFiles, "EARevolution.APX");
    CString menuPath = m_quickMenuSystemPath;
    if(menuPath.IsEmpty())
        menuPath = m_gStatus->m_loadedMenuSystemPath;
    if(menuPath.IsEmpty())
        menuPath = QuickDeployJoinPath(gameFiles, "EARevolution.MOD");
    CString missingFiles;
    if(!QuickDeployFileExists(entityPath)) missingFiles += "\n  EDB: " + entityPath;
    if(!QuickDeployFileExists(itemPath)) missingFiles += "\n  APX: " + itemPath;
    if(!QuickDeployFileExists(menuPath)) missingFiles += "\n  MOD: " + menuPath;
    if(!missingFiles.IsEmpty())
    {
        MessageBox("The wizard needs the saved EDB, APX, and MOD files before it can make a backed-up update:" +
                   missingFiles,
                   "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    CSqlConnectionDlg connectionDialog;
    if(connectionDialog.DoModal() != IDOK)
        return;
    CDBConnectionSystem sql(connectionDialog.m_ipAddress, connectionDialog.m_database,
                            connectionDialog.m_username, connectionDialog.m_password, NULL);
    if(!sql.m_connectionEstablished)
    {
        MessageBox("The AXServer SQL connection could not be established. No files have been changed.",
                   "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    QuickRaceStartScriptRow sourceStartRow;
    CString sqlError;
    if(!QuickRaceLoadStartScript(&sql, source->m_uniqueId, sourceStartRow, sqlError))
    {
        MessageBox(sqlError, "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }
    BOOL targetRowIsFree = FALSE;
    if(!QuickRaceTargetStartScriptIsFree(&sql, newRaceID, targetRowIsFree, sqlError) || !targetRowIsFree)
    {
        MessageBox(sqlError, "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    CString confirm;
    confirm.Format("Create \"%s\" as TRC_%d, copying TRC_%d \"%s\"?\n\n"
                   "The wizard will copy the character and its %d item binding(s), add a creation page after %s, "
                   "and clone the validated AXServer start row.\n\n"
                   "Files to update:\n  %s\n  %s\n  %s\n\n"
                   "SQL target: tblServerScripts (id=0, gameSpecific=ENDLESS, edbspecific=%d).\n\nProceed?",
                   (const char *)newName, newRaceID, source->m_uniqueId, (const char *)source->List_name,
                   QuickRaceCountItemBindings(items, source->m_uniqueId), (const char *)sourceMenu->m_name,
                   (const char *)entityPath, (const char *)itemPath, (const char *)menuPath, newRaceID);
    if(MessageBox(confirm, "New Race Wizard", MB_YESNO | MB_ICONQUESTION) != IDYES)
        return;

    CString entityBackup, itemBackup, menuBackup;
    CString backupError;
    if(!EngineXCreateDatabaseBackup(entityPath, entityBackup, backupError) ||
       !EngineXCreateDatabaseBackup(itemPath, itemBackup, backupError) ||
       !EngineXCreateDatabaseBackup(menuPath, menuBackup, backupError))
    {
        MessageBox("The wizard stopped because a dated database backup could not be created. "
                   "No local database files have been changed.\n\n" + backupError,
                   "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    CMovementObj *newEntity = QuickRaceCloneCharacter(source, newName, newRaceID, m_gStatus);
    if(!newEntity)
    {
        MessageBox("The selected character could not be cloned. No database files have been changed.",
                   "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }
    CMenuWindow *newMenu = NULL;
    if(!QuickRaceBuildMenuClone(sourceMenu, sourceMenu->m_visualID, oldNextMenuID,
                                newMenuID, newRaceID, newName, m_gStatus->m_3dDevice,
                                m_gStatus->m_menuDatabase, &newMenu, menuError))
    {
        delete newEntity;
        MessageBox(menuError, "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    std::vector<QuickRaceAddedItemRef> addedRefs;
    std::vector<QuickRaceMenuScriptSwap> scriptSwaps;
    BOOL entityAdded = FALSE;
    BOOL menuAdded = FALSE;
    int itemsCopied = 0;
    int bindingsCopied = 0;
    BOOL readyToSave = TRUE;
    BOOL sqlWriteNeedsReview = FALSE;

    m_gStatus->m_entObjList->AddTail(newEntity);
    entityAdded = TRUE;
    if(newEntity->List_skeletalObject)
    {
        InitFXSahdersOnSkeletal(newEntity->List_skeletalObject);
        m_gStatus->SkeletalTextureReassignmentWCompile(newEntity->List_skeletalObject);
    }
    if(!QuickRaceCopyItemBindings(items, source->m_uniqueId, newRaceID,
                                  addedRefs, itemsCopied, bindingsCopied))
    {
        sqlError = "The source item bindings could not be copied to the new race.";
        readyToSave = FALSE;
    }

    if(readyToSave)
    {
        m_gStatus->m_menuDatabase->AddTail(newMenu);
        menuAdded = TRUE;
        if(!QuickRaceReplaceScriptArgument(&sourceNextButton->m_clickScript,
                                           "OpenMenu", 0, newMenuID, &scriptSwaps) ||
           !QuickRaceReplaceScriptArgument(&oldNextPreviousButton->m_clickScript,
                                           "OpenMenu", 0, newMenuID, &scriptSwaps))
        {
            sqlError = "The existing race-navigation links could not be safely updated.";
            readyToSave = FALSE;
        }
    }

    if(readyToSave && !QuickRaceSaveEntityDatabase(m_gStatus, entityPath, sqlError))
        readyToSave = FALSE;
    if(readyToSave && !QuickRaceSaveItemDatabase(items, itemPath, sqlError))
        readyToSave = FALSE;
    if(readyToSave && !QuickRaceSaveMenuSystem(m_gStatus, menuPath, sqlError))
        readyToSave = FALSE;

    if(readyToSave)
    {
        targetRowIsFree = FALSE;
        if(!QuickRaceTargetStartScriptIsFree(&sql, newRaceID, targetRowIsFree, sqlError) ||
           !targetRowIsFree)
            readyToSave = FALSE;
    }

    if(readyToSave)
    {
        CString scriptName = newName + " Start Config";
        if(!sourceStartRow.scriptName.IsEmpty())
            scriptName = newName + " - " + sourceStartRow.scriptName;
        CString description = sourceStartRow.description;
        if(description.IsEmpty())
            description = "Character start configuration for " + newName + ".";
        else
        {
            CString sourceName = source->List_name;
            int sourceNamePosition = description.Find(sourceName);
            if(sourceNamePosition >= 0 && !sourceName.IsEmpty())
                description.Replace(sourceName, newName);
            else
                description += " (race: " + newName + ")";
        }
        CString nameLiteral = QuickRaceSqlStringLiteral(scriptName);
        CString descriptionLiteral = QuickRaceSqlStringLiteral(description);
        CString scriptLiteral = QuickRaceSqlStringLiteral(sourceStartRow.scriptData);
        CString insertQuery;
        insertQuery.Format("INSERT INTO [tblServerScripts] "
                           "([id], [scriptName], [Description], [scriptData], [scriptBasis], [gameSpecific], [edbspecific]) "
                           "VALUES (0, %s, %s, %s, %d, 'ENDLESS', %d)",
                           (const char *)nameLiteral, (const char *)descriptionLiteral,
                           (const char *)scriptLiteral, sourceStartRow.scriptBasis, newRaceID);
        BOOL insertAccepted = sql.ExecQuery(insertQuery);

        CString verifyQuery;
        verifyQuery.Format("SELECT COUNT(*) AS RowCount FROM [tblServerScripts] "
                           "WHERE [id] = 0 AND [gameSpecific] = 'ENDLESS' AND [edbspecific] = %d",
                           newRaceID);
        int verifiedRows = -1;
        BOOL verificationSucceeded = QuickRaceQueryInt(&sql, verifyQuery, verifiedRows);
        if(!insertAccepted || !verificationSucceeded || verifiedRows != 1)
        {
            sqlWriteNeedsReview = !verificationSucceeded || verifiedRows != 0;
            if(!insertAccepted)
                sqlError = "AXServer SQL reported that the start-row insert failed.";
            else if(!verificationSucceeded)
                sqlError = "The AXServer SQL insert result could not be verified.";
            else
                sqlError.Format("AXServer SQL verification found %d start row(s) for the new TRC_%d; expected one.",
                                verifiedRows, newRaceID);
            readyToSave = FALSE;
        }
    }

    if(!readyToSave)
    {
        QuickRaceRollbackInMemory(m_gStatus, addedRefs, newEntity, entityAdded,
                                  newMenu, menuAdded, scriptSwaps);
        CString restoreErrors;
        CString restoreError;
        if(!QuickRaceRestoreFile(entityPath, entityBackup, restoreError))
            restoreErrors += "\n  " + restoreError;
        restoreError.Empty();
        if(!QuickRaceRestoreFile(itemPath, itemBackup, restoreError))
            restoreErrors += "\n  " + restoreError;
        restoreError.Empty();
        if(!QuickRaceRestoreFile(menuPath, menuBackup, restoreError))
            restoreErrors += "\n  " + restoreError;

        CString failure = "The New Race Wizard did not finish. Its in-memory changes were rolled back, "
                          "and the original local files were restored from their dated backups.\n\n" + sqlError;
        if(!restoreErrors.IsEmpty())
            failure += "\n\nRestore problems:" + restoreErrors;
        if(sqlWriteNeedsReview)
            failure += "\n\nThe AXServer SQL rows may need manual review before you retry; local files were rolled back, "
                       "but the SQL insert may have been committed or another writer may have created the target row.";
        failure += "\n\nBackups remain at:\n" + entityBackup + "\n" + itemBackup + "\n" + menuBackup;
        MessageBox(failure, "New Race Wizard", MB_OK | MB_ICONWARNING);
        return;
    }

    QuickRaceCommitMenuScriptSwaps(scriptSwaps);
    addedRefs.clear();
    newEntity = NULL;
    newMenu = NULL;
    m_quickEntityDatabasePath = entityPath;
    m_gStatus->m_loadedEntityDatabasePath = entityPath;
    m_quickMenuSystemPath = menuPath;
    m_gStatus->m_loadedMenuSystemPath = menuPath;
    EnitityInList();
    RefreshMenuList(-1);

    CString result;
    result.Format("Created TRC_%d \"%s\" from TRC_%d.\n\n"
                  "%d item(s), with %d copied binding(s), are now race-enabled.\n"
                  "Creation page %d was inserted after \"%s\" in the race cycle.\n"
                  "The id=0 / ENDLESS SQL start row is registered for edbspecific=%d.\n\n"
                  "Updated files have dated backups:\n%s\n%s\n%s\n\n"
                  "Use Quick > Send To Game to copy the EDB, APX, MOD, and textures into Core and C:\\ea server.",
                  newRaceID, (const char *)newName, source->m_uniqueId,
                  itemsCopied, bindingsCopied, newMenuID, (const char *)sourceMenu->m_name, newRaceID,
                  (const char *)entityBackup, (const char *)itemBackup, (const char *)menuBackup);
    MessageBox(result, "New Race Wizard", MB_OK | MB_ICONINFORMATION);
}

#endif //_ClientOutput
