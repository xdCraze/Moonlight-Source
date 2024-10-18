#include "../Utils.h"
#include <iostream>
#include <intrin.h>
#include <vector>  // For std::vector
#include <list>    // For std::list
#include <string>  // For std::string
#include "../BNM-IL2CPP/Structures/IL2CPP/String.hpp"

std::string clean_string(std::string string)
{
    std::vector<char> bytes(string.begin(), string.end());
    bytes.push_back('\0');
    std::list<char> chars;
    for (byte byte : bytes)
    {
        if (byte)
        {
            chars.push_back(byte);
        }
    }
    std::string clean(chars.begin(), chars.end());
    return clean;
}
 
// Params
struct AnalyticsParams {
    int field1;
    int field2;
    int field3;
    int field4;
    int field5;
    int field6;
    int field7;
    bool field8;
    int field9;
    int field10;
    bool field11;
    int field12;
    int field13;
    bool field14;
    int field15;
    int field16;
    int field17;
    bool field18;
    int64_t field19;
    int field20;
    bool field21;
    bool field22;
    bool field23;
    bool field24;
    int field25;
};

AnalyticsParams offerwallParams = {
0,
0x18,
0x82,
0,
0,
0,
0,
false,
0,
0,
false,
0,
0,
false,
0,
0,
0,
false,
0,
0,
false,
false,
false,
true,
0
};

// Pointers
void (*SetXray) (void*, bool);

void (*AddWeapon)(IL2CPP::String*, bool, bool);
void (*AddWeapons)(IL2CPP::String*, bool, bool);
void (*AddGadget)(IL2CPP::String*, int);
void (*AddPets)(IL2CPP::String*, int);
void (*AddWear)(IL2CPP::String*);

void (*SendChat)(void*, IL2CPP::String*, bool, IL2CPP::String*);

void* (*Instance)();
void (*AddCurrency)(void*, IL2CPP::String*, int, int, bool, bool, AnalyticsParams);
void (*PixelPass)(void*);
void (*Crowns)(void*, int);

void Pointers()
{
    SetXray = (void(*)(void*, bool))(RunBackend.baseAddr + 0x1ba36d0); // class Player_move_c : Rilisoft | Void 4 from Awake()

    AddWeapon = (void (*)(IL2CPP::String*, bool, bool))(RunBackend.baseAddr + 0x20b3fc0);
    AddWeapons = (void (*)(IL2CPP::String*, bool, bool))(RunBackend.baseAddr + 0x20b3fc0);
    AddGadget = (void (*)(IL2CPP::String*, int))(RunBackend.baseAddr + 0x1f4c400);
    AddPets = (void (*)(IL2CPP::String*, int))(RunBackend.baseAddr + 0x20adfc0);

    AddWear = (void (*)(IL2CPP::String*))(RunBackend.baseAddr + 0x20adbc0);

    SendChat = (void (*)(void*, IL2CPP::String*, bool, IL2CPP::String*))(RunBackend.baseAddr + 0x1b60de0);

    AddCurrency = (void (*)(void*, IL2CPP::String*, int, int, bool, bool, AnalyticsParams))(RunBackend.baseAddr + 0xa507b0); // class Progress.丝丆业丝与丂丁丄丐 : System.Object | System.Boolean 4
    Instance = (void*(*)())(RunBackend.baseAddr + 0xa76570); // class Progress.丝丆业丝与丂丁丄丐 : System.Object | Static.Progress

    PixelPass = (void (*)(void*))(RunBackend.baseAddr + 0xa615a0);
    Crowns = (void (*)(void*, int))(RunBackend.baseAddr + 0xa78200);
}

// Some goofy ahh stuff
void(*PixelTime_o)(void* instance);
void  PixelTime_h(void* instance)
{
    if (instance != nullptr)
    {

        if (RunBackend.AddCurrency)
        {
            AddCurrency(Instance(), IL2CPP::String::New(RunBackend.CurrencyList[RunBackend.CurrencyType]), RunBackend.CurrencyAmmount, 1, false, false, offerwallParams);
            RunBackend.AddCurrency = false;
        }

        if (RunBackend.ActivatePass)
        {
            PixelPass(Instance());
            RunBackend.ActivatePass = false;
        }

        if (RunBackend.AddCrowns)
        {
            Crowns(Instance(), RunBackend.CrownsAmmount);
            RunBackend.AddCrowns = false;
        }

        if (RunBackend.AddWeapon)
        {
            AddWeapon(IL2CPP::String::New(RunBackend.WeaponInput), true, true);
            RunBackend.AddWeapon = false;
        }


        if (RunBackend.Ban)
        {
            for (int i = 0; i < 1; i++)
            {
                AddWeapon(IL2CPP::String::New(RunBackend.BanList[i]), false, false);
                RunBackend.Ban = false;
            }
        }

        if (RunBackend.Loadout)
        {
            for (int i = 0; i < 6; i++)
            {
                AddWeapon(IL2CPP::String::New(RunBackend.WeaponsLoadout[i]), true, true);
            }

            for (int i = 0; i < 5; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.ArmorLoadout[i]));
            }

            for (int i = 0; i < 2; i++)
            {
                AddGadget(IL2CPP::String::New(RunBackend.GadgetLoadout[i]), 1);
            }

            RunBackend.Loadout = false;
        }

        // bulk adder shi
        if (RunBackend.AddWeapons)
        {
            for (int i = RunBackend.AddWeaponsBeginning; i < RunBackend.AddWeaponsEnd; i++)
            {
                AddWeapons(IL2CPP::String::New(RunBackend.WeaponList[i]), true, true);
                RunBackend.AddWeapons = false;
            }
        }

        if (RunBackend.AddArmor)
        {
            for (int i = 0; i < 29; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.ArmorList[i]));
                RunBackend.AddArmor = false;
            }
        }

        if (RunBackend.AddHats)
        {
            for (int i = 0; i < 30; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.HatList[i]));
                RunBackend.AddHats = false;
            }
        }

        if (RunBackend.AddBoots)
        {
            for (int i = 0; i < 38; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.BootList[i]));
                RunBackend.AddBoots = false;
            }
        }

        if (RunBackend.AddMasks)
        {
            for (int i = 0; i < 40; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.MaskList[i]));
                RunBackend.AddMasks = false;
            }
        }

        if (RunBackend.AddCapes)
        {
            for (int i = 0; i < 32; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.CapeList[i]));
                RunBackend.AddCapes = false;
            }
        }

        if (RunBackend.AddVeteran)
        {
            for (int i = 0; i < 5; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.VeteranList[i]));
                RunBackend.AddVeteran = false;
            }
        }

        if (RunBackend.AddAvatars)
        {
            for (int i = 0; i < 472; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.AvatarList[i]));
                RunBackend.AddAvatars = false;
            }
        }

        if (RunBackend.AddShovels)
        {
            for (int i = 0; i < 103; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.ShovelList[i]));
                RunBackend.AddShovels = false;
            }
        }

        if (RunBackend.AddGliders)
        {
            for (int i = 0; i < 123; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.GliderList[i]));
                RunBackend.AddGliders = false;
            }
        }

        if (RunBackend.AddTrails)
        {
            for (int i = 0; i < 137; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.TrailList[i]));
                RunBackend.AddTrails = false;
            }
        }

        if (RunBackend.AddVehicles)
        {
            for (int i = 0; i < 67; i++)
            {
                AddWear(IL2CPP::String::New(RunBackend.VehicleNames[i]));
                RunBackend.AddVehicles = false;
            }
        }

        if (RunBackend.AddPets)
        {
            for (int i = 0; i < 101; i++)
            {
                AddPets(IL2CPP::String::New(RunBackend.PetList[i]), 9999);
                RunBackend.AddPets = false;

            }
        }

        if (RunBackend.AddAllGadgets)
        {
            for (int i = 0; i < 48; i++)
            {
                AddGadget(IL2CPP::String::New(RunBackend.GadgetsList[i]), RunBackend.GadgetLevel);
                RunBackend.AddAllGadgets = false;
            }
        }

        if (RunBackend.AddAArmor)
        {
            AddWear(IL2CPP::String::New(RunBackend.ArmorList[RunBackend.ArmorType]));
            RunBackend.AddAArmor = false;
        }

        // individual adder shi
        if (RunBackend.AddGadgets)
        {
            AddGadget(IL2CPP::String::New(RunBackend.GadgetsList[RunBackend.Gadgets]), RunBackend.GadgetLevel);
            RunBackend.AddGadgets = false;
        }

    }

    return PixelTime_o(instance);
}

/*
        ojson __create(int id, ojson params, bool emptyParams = false, bool emptyU = false)
        {
            ojson 2a = {
                {"id", id},
                {"ci", randHex()},
                {"p", json::object()},
                {"h", json::object()},
                {"v", GameVersion},
                {"u", json::array()},
            };

            if (!emptyParams) 2a["p"] = params;
            if (!emptyU) 2a["u"] = { 0 };

            return 2a;
        }
*/



void(*WeaponSounds_о)(void* instance);
void  WeaponSounds(void* instance)
{

    if (RunBackend.KillAllCheck)
    {
        *(bool*)((uint64_t)instance + 0x369) = true; // isFrostSword
        *(float*)((uint64_t)instance + 0x36c) = 5; // frostDamageMultiplier
        *(float*)((uint64_t)instance + 0x370) = 99999999999; // frostRadius
        *(float*)((uint64_t)instance + 0x37c) = 0.5; // frostRadius
    }

    if (RunBackend.FireImmunity)
    {
        *(bool*)((uint64_t)instance + 0x240) = true; // fireImmunity
    }

    if (RunBackend.BleedingImmunity)
    {
        *(bool*)((uint64_t)instance + 0x241) = true; // bleedingImmunity
    }

    if (RunBackend.ToxicImmunity)
    {
        *(bool*)((uint64_t)instance + 0x242) = true; // toxicImmunity
    }

    if (RunBackend.XRay)
    {
        *(bool*)((uint64_t)instance + 0xcd) = true; // zoomXray
    }

    if (RunBackend.PolyMorpher)
    {
        *(bool*)((uint64_t)instance + 0x2ec) = true; // polymorpher
        *(bool*)((uint64_t)instance + 0x12f) = true; // bazooka
        *(float*)((uint64_t)instance + 0x2f0) = 99999; // polymorphDuarationTime
        *(int*)((uint64_t)instance + 0x2f4) = 1; // polymorphType
        *(float*)((uint64_t)instance + 0x2f8) = 999; // polymorphMaxHealth
    }

    if (RunBackend.Points)
    {
        *(bool*)((uint64_t)instance + 0x394) = true; // isBuffPoints
        *(float*)((uint64_t)instance + 0x398) = 110; // buffPointsRevenge
        *(float*)((uint64_t)instance + 0x3b0) = 110; // buffPointsOther
        *(float*)((uint64_t)instance + 0x3a0) = 110; // buffBonusPointsForKil
    }


    if (RunBackend.ArmorRegen)
    {
        *(bool*)((uint64_t)instance + 0x3ec) = true; // isArmorRegeneration
        *(float*)((uint64_t)instance + 0x3f0) = 100; // armorRegenerationPercent
        *(float*)((uint64_t)instance + 0x3f4) = 1; // armorRegenerationTime
    }

    if (RunBackend.Bazooka)
    {
        *(bool*)((uint64_t)instance + 0x12f) = true; // bazooka
    }

    if (RunBackend.GrenadeLauncher)
    {
        *(bool*)((uint64_t)instance + 0x158) = true; // grenadeLauncher
    }

    if (RunBackend.BulletExplode)
    {
        *(bool*)((uint64_t)instance + 0x19d) = true; // bulletExplode
    }

    if (RunBackend.Shotgun)
    {
        *(bool*)((uint64_t)instance + 0x19e) = true; // isShotGun
    }

    if (RunBackend.InfiniteAmmo)
    {
        *(bool*)((uint64_t)instance + 0x469) = true; // isUnlimitedAmmo
    }

    if (RunBackend.DamageMulti)
    {
        *(bool*)((uint64_t)instance + 0x420) = true; // isIncreasedDamageFromKill
        *(float*)((uint64_t)instance + 0x424) = 5; // damageMultiplier
    }

    if (RunBackend.DoubleJump)
    {
        *(bool*)((uint64_t)instance + 0x478) = true; // isDoubleJump
    }

    if (RunBackend.Railgun)
    {
        *(bool*)((uint64_t)instance + 0x19c) = true; // bulletBreakout
        *(bool*)((uint64_t)instance + 0x19f) = true; // bulletSuperBreakout
        *(bool*)((uint64_t)instance + 0x1c0) = false; // railgunStopAtWall
    }

    if (RunBackend.ThreeCatSpam)
    {
        *(bool*)((uint64_t)instance + 0x6a8) = true; // is3CatSpam
        *(bool*)((uint64_t)instance + 0x6a9) = true; // for3CatSpam
    }

    if (RunBackend.Harpoon)
    {
        *(bool*)((uint64_t)instance + 0x2b8) = true; // harpoon
    }

    if (RunBackend.Flamethrower)
    {
        *(bool*)((uint64_t)instance + 0x34c) = true; // flamethrower
    }

    if (RunBackend.CoinDrop)
    {
        *(bool*)((uint64_t)instance + 0x330) = true; // isCoinDrop
        *(float*)((uint64_t)instance + 0x334) = 100; // coinDropChance
    }

    if (RunBackend.MechShoot)
    {
        *(bool*)((uint64_t)instance + 0x528) = true; // isWeaponInsideHands
    }


    return WeaponSounds_о(instance);
}

 void (*AmmoInClip_о)(void* instance, int a);
void  AmmoInClip(void* instance, int a)
{

    if (instance != nullptr && RunBackend.Test)
    {
        return  AmmoInClip_о(instance, 999);
    }

    return AmmoInClip_о(instance, a);
}

void(*Player_move_c_о)(void* instance);
void  Player_move_c(void* instance)
{

    if (RunBackend.ESP)
    {
        SetXray(instance, true);
    }

    if (instance != nullptr)
    {
        if (RunBackend.ChatSend)
        {
            void* nickLabel = (void*)*(uint64_t*)((uint64_t)instance + 0x3d8);
            static const auto getText = (void* (*)(void*))(RunBackend.baseAddr + 0x44f0aa0);
            void* namePtr = getText(nickLabel);

            if (namePtr != nullptr)
            {
                std::string Name = clean_string(((IL2CPP::String*)namePtr)->Content());
                if (Name == "1111")
                {
                    SendChat(instance, IL2CPP::String::New(RunBackend.ChatInput), false, IL2CPP::String::New("0"));
                }

            }

            if (RunBackend.ChatLoop)
            {
                RunBackend.ChatSend = true;
            }

            else
            {
                RunBackend.ChatSend = false;
            }

        }
    }

    return Player_move_c_о(instance);
}

int(*XpSpoof_o)(void* instance, int a, int z, void* b);
int  XpSpoof(void* instance, int a, int z, void* b)
{
    if (instance != nullptr && RunBackend.xpSpoof)
    {
        return  XpSpoof_o(instance, RunBackend.xpSpoofInt, z, b);
    }

    if (instance != nullptr && RunBackend.Level65)
    {
        return  XpSpoof_o(instance, 32645, z, b);
    }

    return XpSpoof_o(instance, a, z, b);
}

int(*Test_o)(void* instance, int a, int z, void* b);
int  Test(void* instance, int a, int z, void* b)
{
    if (instance != nullptr && RunBackend.xpAdder)
    {
        return  Test_o(instance, 10, z, b);
        RunBackend.xpAdder = false;
    }

    return Test_o(instance, a, z, b);
}

int (*PriceModifier_о)(void* instance);
int  PriceModifier(void* instance)
{
    if (RunBackend.PriceModifier)
    {
        return RunBackend.ModifierAmmount;
    }

    return PriceModifier_о(instance);
}

// Define Hook
void Hook(uint64_t offset, LPVOID detour, void* original)
{
    if (MH_CreateHook((LPVOID*)(RunBackend.baseAddr + offset), detour, (LPVOID*)original) == MH_OK)
    {
        MH_EnableHook((LPVOID*)(RunBackend.baseAddr + offset));
    }
}

// Hooking and whatnot
void Hooks()
{
    Hook(0x6cda90, &WeaponSounds, &WeaponSounds_о); // class WeaponSounds Update
    Hook(0x76b8e0, &XpSpoof, &XpSpoof_o); // class ExperienceController
    // Hook(0x7661d0, &Test, &Test_o); // class ExperienceController
    Hook(0x2222f10, &PixelTime_h, &PixelTime_o); // class PixelTime Update
    Hook(0x1bfce70, &Player_move_c, &Player_move_c_о); // class Player_move_c : Rilisoft Update
    Hook(0x7f7e40, &PriceModifier, &PriceModifier_о); // class 东丛且丟世丌丐丅世 : System.Object, System.ICloneable

    Pointers();
}