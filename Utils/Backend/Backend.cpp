#include "../Backend/Backend.h"

#include <Windows.h>
#include <shellapi.h>


#include "../Fonts/Byte.h"
#include "../Fonts/FontAwesome.h"


Backend::presentVariable originalPresent;
Backend::presentVariable hookedPresent;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool init = false;

Backend RunBackend;

bool Backend::DirectXPresentHook()
{
	ZeroMemory(&m_gSwapChainDescription, sizeof(m_gSwapChainDescription));

	m_gSwapChainDescription.BufferCount = 2;
	m_gSwapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_gSwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_gSwapChainDescription.OutputWindow = GetForegroundWindow();
	m_gSwapChainDescription.SampleDesc.Count = 1;
	m_gSwapChainDescription.Windowed = TRUE;
	m_gSwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	HRESULT createDevice = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, m_gFeatureLevels, 2, D3D11_SDK_VERSION, &m_gSwapChainDescription, &m_gSwapChain, &m_gDevice, nullptr, nullptr);
		
	if (FAILED(createDevice)) 
		return false; // dont return false make an endless cycle (only if u wanna go cpu boom) 

	void** DX11Vtable = *reinterpret_cast<void***>(m_gSwapChain);

	UnloadDevices(false); // don't need to reset mainrendertargetview
	hookedPresent = (Backend::presentVariable)DX11Vtable[8]; // 8. virtual table is present

	return true;
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	ImGuiIO& io = ImGui::GetIO();

	if (RunBackend.m_bOpenMenu)
	{

		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	if (RunBackend.m_bOpenMenu && (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_MOUSEMOVE || wParam != 'W' && wParam != 'A' && wParam != 'S' && wParam != 'D' && wParam != VK_SHIFT && wParam != VK_CONTROL && wParam != VK_TAB && wParam != VK_SPACE))
		return false;

	return CallWindowProc(RunBackend.m_goriginalWndProc, hWnd, uMsg, wParam, lParam);
}

void Backend::LoadImGui(HWND window, ID3D11Device* device, ID3D11DeviceContext* context)
{
	ImGui::CreateContext(); // creating the context cus we need imgui
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange; // dont change cursors
	ImGui_ImplWin32_Init(window); // which window u wanna draw your imgui huh???
	ImGui_ImplDX11_Init(device, context); // u need the device's context since u can't draw with only device, thanx dx11

	// Load Fonts
	ImFontConfig font_config;
	font_config.PixelSnapH = false;
	font_config.OversampleH = 8;
	font_config.OversampleV = 8;
	font_config.RasterizerMultiply = 1.2f;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
		0x2DE0, 0x2DFF, // Cyrillic Extended-A
		0xA640, 0xA69F, // Cyrillic Extended-B
		0xE000, 0xE226, // icons
		0,
	};

	font_config.GlyphRanges = ranges;

	Fonts::Medium = io.Fonts->AddFontFromMemoryTTF(StolzlFont, sizeof(StolzlFont), 15.0f, &font_config, ranges);
	Fonts::Bold = io.Fonts->AddFontFromMemoryTTF(StolzlBold, sizeof(StolzlBold), 17.0f, &font_config, ranges);

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true; icons_config.OversampleH = 8; icons_config.OversampleV = 8;

	Fonts::FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 16.0f, &icons_config, icons_ranges);

}

void Backend::DrawImGui(ID3D11DeviceContext* context, ID3D11RenderTargetView* targetview)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	if (m_bOpenMenu)
	{
		ClipCursor(NULL);
		ImGui::GetIO().MouseDrawCursor = true;

		if (RunBackend.bgDim)
		{
			ImGui::StyleColorsLight();
			ImGui::Begin("bgDim", &RunBackend.bgDim, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBringToFrontOnFocus);
			{
				ImGui::SetWindowSize(ImVec2(5000, 5000));
				ImGui::SetWindowPos(ImVec2(0 - 1000, 0 - 1000));
			}
			ImGui::End();
		}

		static Heads Tab{ General };
		const char* TabName = Tab == General ? "General" : Tab == Exploits ? "Exploits" : Tab == Account ? "Account" : Tab == Visuals ? "Visuals" : Tab == Misc ? "Miscellaneous" : 0;

		ImGui::StyleColorsMoonlight();
		ImGui::Begin("Moonlight", nullptr,  ImGuiWindowFlags_NoDecoration |ImGuiWindowFlags_NoSavedSettings);
		{
			ImGui::SetWindowSize(ImVec2(800, 600));
			auto draw = ImGui::GetWindowDrawList();
			auto pos = ImGui::GetWindowPos();
			auto size = ImGui::GetWindowSize();

			draw->AddCircleFilled(ImVec2(pos.x + 28, pos.y + size.y - 30), 20, ImColor(23, 23, 23), 100);
			draw->AddText(Fonts::FontAwesome, 25.0f, ImVec2(pos.x + 18, pos.y + size.y - 40), ImColor(33, 33, 33), ICON_FA_USER);

			ImGui::GetBackgroundDrawList()->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + 800, pos.y + 600), ImColor(60, 120, 255, 255), 100, ImVec2(0, 0), 10);

			draw->AddRectFilled(ImVec2(pos.x + 54, pos.y + 50), ImVec2(pos.x + size.x, pos.y + size.y), ImColor(18, 18, 18), 10.0f, ImDrawFlags_RoundCornersBottomRight);
			draw->AddShadowRect(ImVec2(0, 0), ImVec2(0, 0), ImColor(50, 50, 50), 1500, ImVec2(pos.x + size.x, pos.y + size.y));


			ImGui::SetCursorPos({ 6, 66 });
			ImGui::BeginGroup(); {
				ImGui::PushFont(Fonts::FontAwesome);

				if (ImGui::CustomTab(ICON_FA_CROSSHAIRS, 12, 41, Tab == General)) { Tab = General;}
				if (ImGui::CustomTab(ICON_FA_ID_CARD, 10, 41, Tab == Account)) { Tab = Account; }
				if (ImGui::CustomTab(ICON_FA_EYE, 11, 41, Tab == Visuals)) { Tab = Visuals; }
				if (ImGui::CustomTab(ICON_FA_LIST, 12, 41, Tab == Misc)) { Tab = Misc; }


				ImGui::PopFont();
			} ImGui::EndGroup();

			ImGui::SetCursorPos(ImVec2(63, 50));
			ImGui::BeginChild("scroll fix by @dsyq", ImVec2(size.x - 63, 0), true);
			{
				switch (Tab)
				{
				case General:
					ImGui::SetCursorPos(ImVec2(0, 10));
					ImGui::MenuChild("Main", ImVec2(255, 300));
					{
						ImGui::CustomCheckbox("Kill All", ImColor(255, 200, 0), &RunBackend.KillAllCheck);
						ImGui::CustomCheckbox("Polymorpher", ImColor(255, 200, 0), &RunBackend.PolyMorpher);
						ImGui::CustomCheckbox("Infinite Points", ImColor(255, 200, 0), &RunBackend.Points);
						ImGui::CustomCheckbox("Armor Regeneration", ImColor(255, 255, 255), &RunBackend.ArmorRegen);
						ImGui::CustomCheckbox("Double Jump", ImColor(255, 255, 255), &RunBackend.DoubleJump);
						ImGui::CustomCheckbox("Coin Drop", ImColor(255, 255, 255), &RunBackend.CoinDrop);
						ImGui::CustomCheckbox("X-Ray on Scopes", ImColor(255, 255, 255), &RunBackend.XRay);
					}
					ImGui::EndChild();

					ImGui::MenuChild("Bullet Modifier", ImVec2(255, 230));
					{
						ImGui::CustomCheckbox("Infinite Ammo", ImColor(255, 255, 255), &RunBackend.InfiniteAmmo);
						ImGui::CustomCheckbox("Bullet Explode", ImColor(255, 255, 255), &RunBackend.BulletExplode);
						ImGui::CustomCheckbox("Shotgun", ImColor(255, 255, 255), &RunBackend.Shotgun);
						ImGui::CustomCheckbox("Harpoon", ImColor(255, 255, 255), &RunBackend.Harpoon);
						ImGui::CustomCheckbox("Flamethrower", ImColor(255, 255, 255), &RunBackend.Flamethrower);
						ImGui::CustomCheckbox("Shoot Through Walls", ImColor(255, 255, 255), &RunBackend.Railgun);
					}
					ImGui::EndChild();

					ImGui::SetCursorPos(ImVec2(265, 10));
					ImGui::MenuChild("Exploits", ImVec2(255, 300));
					{
						ImGui::CustomCheckbox("Fire Immunity", ImColor(255, 255, 255), &RunBackend.FireImmunity);
						ImGui::CustomCheckbox("Bleeding Immunity", ImColor(255, 255, 255), &RunBackend.BleedingImmunity);
						ImGui::CustomCheckbox("Poison Immunity", ImColor(255, 255, 255), &RunBackend.ToxicImmunity);
					}
					ImGui::EndChild();

					break;

				case Account:

					ImGui::SetCursorPos(ImVec2(0, 10));
					ImGui::MenuChild("Currency Adder", ImVec2(255, 220));
					{
						ImGui::Combo("Currency Type", &RunBackend.CurrencyType, RunBackend.CurrencyList, IM_ARRAYSIZE(RunBackend.CurrencyList));

						ImGui::SliderInt("Currency Ammount", &RunBackend.CurrencyAmmount, 0, 1000, "%d", ImGuiSliderFlags_None);
						if (ImGui::CustomButton("##AddCurrency", "Add Currency", 100, 30)) { RunBackend.AddCurrency = true; }

						ImGui::Text("Adding over 4k per click could put\nyour account at risk!");
					}
					ImGui::EndChild();

					ImGui::MenuChild("XP Adder", ImVec2(255, 307));
					{
						ImGui::CustomCheckbox("XP Adder", ImColor(255, 255, 255), &RunBackend.xpSpoof);
						ImGui::SliderInt("XP Amount", &RunBackend.xpSpoofInt, 1, 200, "%d", ImGuiSliderFlags_None);
					}
					ImGui::EndChild();

					ImGui::SetCursorPos(ImVec2(265, 10));
					ImGui::MenuChild("Weapon Adder", ImVec2(460, 190));
					{
						ImGui::Indent(25);
						ImGui::Spacing(); ImGui::Spacing();
						ImGui::CustomInputText("##test", "Weapon String", 10, 35, 305, WeaponInput, 50, 0);
						ImGui::SameLine();
						if (ImGui::CustomButton("##Addweapon", "Add Weapon", 130, 30)) { RunBackend.AddWeapon = true; }
						ImGui::Text("Only add the weapons you need!                             ");
						ImGui::SameLine();
						if (ImGui::CustomButton("##Strings", "Weapon Strings", 110, 30)) { ShellExecute(NULL, L"open", L"https://yeetdisdude.xyz/weaponstrings", 0, 0, SW_SHOWNORMAL); }

						ImGui::Unindent();
					}
					ImGui::EndChild();

					ImGui::SetCursorPos(ImVec2(265, 204));
					ImGui::MenuChild("Gadgets Adder", ImVec2(460, 175));
					{
						ImGui::Combo("Gadgets", &RunBackend.Gadgets, RunBackend.GadgetsList, IM_ARRAYSIZE(RunBackend.GadgetsList));
						if (ImGui::CustomButton("##Addgadgets", "Add Gadget", 100, 30)) { RunBackend.AddGadgets = true; }
					}
					ImGui::EndChild();

					ImGui::SetCursorPos(ImVec2(265, 383));
					ImGui::MenuChild("Wear Adder", ImVec2(460, 160));
					{
						if (ImGui::CustomButton("##Addallarmor", "Add All Armor", 120, 30)) { RunBackend.AddArmor = true; }
						ImGui::SameLine();
						if (ImGui::CustomButton("##Addallhats", "Add All Hats", 120, 30)) { RunBackend.AddHats = true; }
						ImGui::SameLine();
						if (ImGui::CustomButton("##Addallboots", "Add All Boots", 120, 30)) { RunBackend.AddBoots = true; }

						if (ImGui::CustomButton("##Addallmasks", "Add All Masks", 120, 30)) { RunBackend.AddMasks = true; }
						ImGui::SameLine();
						if (ImGui::CustomButton("##Addallcapes", "Add All Capes", 120, 30)) { RunBackend.AddCapes = true; }
						ImGui::SameLine();
						if (ImGui::CustomButton("##Addveteran", "Add Veteran Set", 120, 30)) { RunBackend.AddVeteran = true; }

						ImGui::Indent(5);
						ImGui::Text("Only add Veteran Set once you are level 65!");
						ImGui::Unindent();
					}

					break;

				case Visuals:

					ImGui::SetCursorPos(ImVec2(0, 10));
					ImGui::MenuChild("ESP", ImVec2(255, 150));
					{
						ImGui::CustomCheckbox("Chams", ImColor(255, 255, 255), &RunBackend.ESP);

					}
					ImGui::EndChild();

					break;

				case Misc:

					ImGui::SetCursorPos(ImVec2(0, 10));
					ImGui::MenuChild("Menu", ImVec2(255, 180));
					{
						ImGui::CustomCheckbox("Background Dim", ImColor(255, 255, 255), &RunBackend.bgDim);
						// ImGui::CustomCheckbox("Watermark", ImColor(255, 255, 255), &RunBackend.Watermark); // Not available for the free version
						if (ImGui::CustomButton("##unload", "Unload", 120, 30))
						{
							RunBackend.Unload(); // unload everything
						}
					}
					ImGui::EndChild();

					ImGui::MenuChild("Socials", ImVec2(255, 180));
					{
						if (ImGui::CustomButton("##yt", "YouTube", 120, 30))
						{
							ShellExecute(NULL, L"open", L"https://youtube.com/@xdcraze", 0, 0, SW_SHOWNORMAL);
						}

						if (ImGui::CustomButton("##dsc", "Discord", 120, 30))
						{
							ShellExecute(NULL, L"open", L"https://discord.gg/Uw48gqKxNF", 0, 0, SW_SHOWNORMAL);
						}

					}
					ImGui::EndChild();

					ImGui::MenuChild("Experimental", ImVec2(255, 165));
					{
						if (ImGui::CustomButton("##ban", "Ban Button!", 120, 30)) { RunBackend.Ban = true; }
						if (ImGui::CustomButton("##loadout", "Craze's Loadout", 120, 30)) { RunBackend.Loadout = true; }
						ImGui::CustomCheckbox("Instant Level 65", ImColor(255, 0, 0), &RunBackend.Level65);
						ImGui::Text("Play a match with this enabled, and\nat the end you will get level 65!");

					}
					ImGui::EndChild();

					ImGui::SetCursorPos(ImVec2(265, 10));
					ImGui::MenuChild("Credits", ImVec2(size.x - 340, 180));
					{
						ImGui::Text("Dc$ | Helped with cheat functions + scrolling.");
						ImGui::Text("zzmate | DX11 base. (https://github.com/zzzmate/DX11-Hook-ImGui)");
						ImGui::Spacing();
						ImGui::Text("HONORABLE MENTIONS");
						ImGui::Spacing();
						ImGui::Text("Chat-GPT | Optimizing code + mouse fix.");
						ImGui::Text("Sxitxma | Pixel Pass features.");
						ImGui::Text("YeetDisDude | Weapon Strings list used.");

					}
					ImGui::EndChild();

					ImGui::SetCursorPos(ImVec2(265, 194));
					ImGui::MenuChild("Guides", ImVec2(size.x - 340, 350));
					{
						ImGui::Text("HOW TO STAY UNBANNED");
						ImGui::Spacing();
						ImGui::Text("Wait until you're atleast level 3.");
						ImGui::Text("Wait a day or two before modding.");
						ImGui::Text("Add around 3k currency per click.");
						ImGui::Text("Only add around 6k a day.");
						ImGui::Text("Play 2 matches and add 100 XP on each match.");
						ImGui::Text("Only add around 50 weapons a day. 200 is the max.");
						ImGui::Text("Adding all wear is fine, but don't add all of it in 1 day.");
						ImGui::Text("Do NOT add Veteran Set until you're level 65.");
						ImGui::Text("I recommend running Ban Trace Cleaner before you create\na new account.");
						ImGui::Spacing();
						ImGui::Text("Link:");
						if (ImGui::CustomButton("##bancleaner", "Ban Trace Cleaner", 125, 30))
						{
							ShellExecute(NULL, L"open", L"https://github.com/YeetDisDude/bantracecleaner/releases/tag/1.1", 0, 0, SW_SHOWNORMAL);
						}
					}
					ImGui::EndChild();

					break;

				}

			}
			ImGui::EndChild();

			draw->AddLine(ImVec2(pos.x, pos.y + 47), ImVec2(pos.x + size.x, pos.y + 47), ImColor(5, 5, 5), 2.0f);
			draw->AddRectFilled(ImVec2(pos.x + 54, pos.y), ImVec2(pos.x + size.x, pos.y + 47), ImColor(15, 15, 15), 10.0f, ImDrawFlags_RoundCornersTopRight);
			draw->AddLine(ImVec2(pos.x + 54, pos.y - 5), ImVec2(pos.x + 54, pos.y + size.y + 5), ImColor(5, 5, 5), 2.0f);

			draw->AddShadowRect(ImVec2(0, 0), ImVec2(0, 0), ImColor(60, 120, 255), 100, ImVec2(pos.x + 25, pos.y + 25));
			draw->AddText(Fonts::FontAwesome, 25.0f, ImVec2(pos.x + 20, pos.y + 12), ImColor(60, 120, 255), ICON_FA_MOON);

			draw->AddText(Fonts::Medium, 17.0f, ImVec2(pos.x + 63, pos.y + 14), ImColor(255, 255, 255), TabName);

			draw->AddText(Fonts::Medium, 17.0f, ImVec2(pos.x + size.x - 228, pos.y + 14), ImColor(255, 255, 255), "Moonlight FREE -");
			draw->AddText(Fonts::Medium, 17.0f, ImVec2(pos.x + size.x - 100, pos.y + 14), ImColor(60, 120, 255), "Pixel Gun 3D");

		}
		ImGui::End();
	}


	else
	{
		ImGui::GetIO().MouseDrawCursor = false;
	}

	if (RunBackend.Watermark)
	{
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 650, 25), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(200, 50));

		ImGui::StyleColorsWatermark();
		ImGui::Begin("Watermark", &RunBackend.Watermark, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBringToFrontOnFocus);
		{
			auto draw = ImGui::GetWindowDrawList();
			auto pos = ImGui::GetWindowPos();
			auto size = ImGui::GetWindowSize();

			ImGui::GetBackgroundDrawList()->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + 200, pos.y + 50), ImColor(60, 120, 255, 255), 100, ImVec2(0, 0), 10);
			draw->AddShadowRect(ImVec2(0, 0), ImVec2(0, 0), ImColor(60, 120, 255), 100, ImVec2(pos.x + 25, pos.y + 25));
			draw->AddText(Fonts::FontAwesome, 25.0f, ImVec2(pos.x + 20, pos.y + 12), ImColor(60, 120, 255), ICON_FA_MOON);
			draw->AddText(Fonts::Medium, 17.0f, ImVec2(pos.x + 45, pos.y + 14), ImColor(255, 255, 255), "Moonlight");
			draw->AddText(Fonts::Medium, 17.0f, ImVec2(pos.x + 125, pos.y + 14), ImColor(60, 120, 255), "FREE");
		}
		ImGui::End();

	}

	ImGui::EndFrame();
	ImGui::Render();

	context->OMSetRenderTargets(1, &targetview, NULL);  // 1 render target, render it to our monitor, no dsv
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // drawing the imgui menu
}

void Backend::UnloadImGui()
{
	MH_DisableHook(MH_ALL_HOOKS); // if u making a base on this and using minhook then dont do this, just disable the present hook, not every hook
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Backend::UnloadDevices(bool renderTargetViewReset)
{
	if(renderTargetViewReset) if (m_gMainRenderTargetView) { m_gMainRenderTargetView->Release(); m_gMainRenderTargetView = nullptr; }
	if (m_gPointerContext) { m_gPointerContext->Release(); m_gPointerContext = nullptr; }
	if (m_gDevice) { m_gDevice->Release(); m_gDevice = nullptr; }
	SetWindowLongPtr(m_gWindow, GWLP_WNDPROC, (LONG_PTR)(m_goriginalWndProc));
}

static long __stdcall PresentHook(IDXGISwapChain* pointerSwapChain, UINT sync, UINT flags)
{
	if (!init) {
		if (SUCCEEDED(pointerSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&RunBackend.m_gDevice))) // check if device working 
		{
			RunBackend.m_gDevice->GetImmediateContext(&RunBackend.m_gPointerContext); // need context immediately!!
			pointerSwapChain->GetDesc(&RunBackend.m_gPresentHookSwapChain); // welp we need the presenthook's outputwindow so it's actually ours o_o
			RunBackend.m_gWindow = RunBackend.m_gPresentHookSwapChain.OutputWindow;

			pointerSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RunBackend.m_gPointerBackBuffer); // getting back buffer
			if (RunBackend.m_gPointerBackBuffer != nullptr) RunBackend.m_gDevice->CreateRenderTargetView(RunBackend.m_gPointerBackBuffer, NULL, &RunBackend.m_gMainRenderTargetView); // from backbuffer to our monitor
			RunBackend.m_gPointerBackBuffer->Release(); // don't need this shit anymore, but please comeback the next injection

			RunBackend.LoadImGui(RunBackend.m_gWindow, RunBackend.m_gDevice, RunBackend.m_gPointerContext); // load imgui!!!
			RunBackend.m_goriginalWndProc = (WNDPROC)SetWindowLongPtr(RunBackend.m_gWindow, GWLP_WNDPROC, (LONG_PTR)WndProc); // i think u need this

			init = true;
		}
		else
			return originalPresent(pointerSwapChain, sync, flags); // returning original too
	}

	if (Utils::keyPressed(OPEN_MENU_KEY))
		RunBackend.m_bOpenMenu = !RunBackend.m_bOpenMenu;

	RunBackend.DrawImGui(RunBackend.m_gPointerContext, RunBackend.m_gMainRenderTargetView); // draw imgui every time
	return originalPresent(pointerSwapChain, sync, flags); // return the original so no stack corruption
}

bool Backend::Load()
{

	RunBackend.DirectXPresentHook(); // this always okay if game directx11
	MH_Initialize(); // aint no error checking cuz if minhook bad then its your problem 

	MH_CreateHook(reinterpret_cast<void**>(hookedPresent), &PresentHook, reinterpret_cast<void**>(&originalPresent)); 
	MH_EnableHook(hookedPresent); // hooking present
	return true;
}

void Backend::Unload()
{
	UnloadImGui(); // imgui unload
	UnloadDevices(true); // unloading all devices
}
