// Generated on Fri Jul 16 02:48:41 2021

namespace altServer
{
    // ********** Funcdefs **********

    // Callback used for timers
    funcdef void TimerCallback();

    // ********** Typedefs **********

    typedef uint32 Hash;

    // ********** Enums **********

    // Types used by base objects
    enum BaseObjectType
    {
        PLAYER = 0,
        VEHICLE = 1,
        BLIP = 2,
        WEBVIEW = 3,
        VOICE_CHANNEL = 4,
        COLSHAPE = 5,
        CHECKPOINT = 6,
        WEBSOCKET_CLIENT = 7,
    };

    // Types used by colshapes
    enum ColShapeType
    {
        SPHERE = 0,
        CYLINDER = 1,
        CIRCLE = 2,
        CUBOID = 3,
        RECT = 4,
        CHECKPOINT = 5,
    };

    // GTA weapon tints
    enum WeaponTint
    {
        NORMAL = 0,
        GREEN = 1,
        GOLD = 2,
        PINK = 3,
        ARMY = 4,
        LSPD = 5,
        ORANGE = 6,
        PLATINUM = 7,
    };

    // GTA weather types
    enum WeatherType
    {
        EXTRA_SUNNY = 0,
        CLEAR = 1,
        CLOUDS = 2,
        SMOG = 3,
        FOGGY = 4,
        OVERCAST = 5,
        RAIN = 6,
        THUNDER = 7,
        CLEARING = 8,
        NEUTRAL = 9,
        SNOW = 10,
        BLIZZARD = 11,
        SNOWLIGHT = 12,
        XMAS = 13,
        HALLOWEEN = 14,
    };

    // ********** Global variables **********

    int defaultDimension;

    int globalDimension;

    string version;

    string branch;

    uint sdkVersion;

    bool debugMode;

    string resourceMain;

    string resourceName;

    // ********** Functions **********

    // Hashes the given string using the joaat algorithm
    Hash ToHash(const string&in value);

    // Gets all players on the server
    array<Player@>@ GetAllPlayers();

    // Gets all vehicles on the server
    array<Vehicle@>@ GetAllVehicles();

    // Gets all entities on the server
    array<Entity@>@ GetAllEntities();

    // Gets the player with the specified ID
    Player@ GetPlayerByID(uint16 id);

    // Gets the vehicle with the specified ID
    Vehicle@ GetVehicleByID(uint16 id);

    // Gets the entity with the specified ID
    Entity@ GetEntityByID(uint16 id);

    // Returns whether the specified resource exists and is started
    bool HasResource(const string&in name);

    // Gets the path to the specified resource
    string GetResourcePath(const string&in name);

    // Starts the specified resource
    void StartResource(const string&in name);

    // Stops the specified resource
    void StopResource(const string&in name);

    // Restarts the specified resource
    void RestartResource(const string&in name);

    // Gets the exports of the specified resource
    dictionary@ GetResourceExports(const string&in name);

    // Gets the total time the server has been running for
    uint GetNetTime();

    // Sets the current server password
    void SetPassword(const string&in password);

    // Logs the specified message to the console
    void Log(const string&in msg);

    // Logs the specified message as a warning to the console
    void LogWarning(const string&in msg);

    // Logs the specified message as an error to the console
    void LogError(const string&in msg);

    // Sets a timeout
    uint SetTimeout(TimerCallback@ callback, uint timeout);

    // Sets a interval
    uint SetInterval(TimerCallback@ callback, uint interval);

    // Sets a next tick handler
    uint NextTick(TimerCallback@ callback);

    // Sets a every tick handler
    uint EveryTick(TimerCallback@ callback);

    // Clears specified timer
    void ClearTimeout(uint timerId);

    // Clears specified timer
    void ClearInterval(uint timerId);

    // Clears specified timer
    void ClearNextTick(uint timerId);

    // Clears specified timer
    void ClearEveryTick(uint timerId);

    // Clears specified timer
    void ClearTimer(uint timerId);

    // Registers an event handler for a local custom event
    void On(const string&in event, const string&in handlerName);

    // Registers an event handler for a remote custom event
    void OnClient(const string&in event, const string&in handlerName);

    // Emits a local event (Max 32 args)
    void Emit(const string&in event, ...);

    // Emits a event to all clients (Max 32 args)
    void EmitToAllClients(const string&in event, ...);

    // Returns whether the specified meta key exists
    bool HasMeta(const string&in key);

    // Sets the specified meta key to the specified value
    bool GetMeta(const string&in key, ?&out outValue);

    // Gets the value of the specified meta key
    void SetMeta(const string&in key, ?&in value);

    // Deletes the specified meta key
    void DeleteMeta(const string&in key);

    // Returns whether the specified synced meta key exists
    bool HasSyncedMeta(const string&in key);

    // Sets the specified synced meta key to the specified value
    bool GetSyncedMeta(const string&in key, ?&out outValue);

    // Gets the value of the specified synced meta key
    void SetSyncedMeta(const string&in key, ?&in value);

    // Deletes the specified synced meta key
    void DeleteSyncedMeta(const string&in key);

    // ********** Objects **********

    // R(ed), G(reen), B(lue), A(lpha) color
    class RGBA
    {
        uint8 r;
        uint8 g;
        uint8 b;
        uint8 a;

        RGBA(uint8 r, uint8 g, uint8 b, uint8 a);
        RGBA toBGRA();
    };

    // Two-dimensional vector
    class Vector2
    {
        float x;
        float y;

        Vector2(float x, float y);
        Vector2(int x, int y);
        Vector2 Add(Vector2 vector);
        Vector2 Add(float x, float y);
        Vector2 Add(float value);
        Vector2 Sub(Vector2 vector);
        Vector2 Sub(float x, float y);
        Vector2 Sub(float value);
        Vector2 Mult(Vector2 vector);
        Vector2 Mult(float x, float y);
        Vector2 Mult(float value);
        float Distance(Vector2 vector);
        float Length();
        Vector2 ToRadians();
        Vector2 ToDegrees();
        string opImplConv() const;
    };

    // Three-dimensional vector
    class Vector3
    {
        float x;
        float y;
        float z;

        Vector3(float x, float y, float z);
        Vector3(int x, int y, int z);
        Vector3 Add(Vector3 vector);
        Vector3 Add(float x, float y, float z);
        Vector3 Add(float value);
        Vector3 Sub(Vector3 vector);
        Vector3 Sub(float x, float y, float z);
        Vector3 Sub(float value);
        Vector3 Mult(Vector3 vector);
        Vector3 Mult(float x, float y, float z);
        Vector3 Mult(float value);
        float Distance(Vector3 vector);
        float Length();
        Vector3 ToRadians();
        Vector3 ToDegrees();
        string opImplConv() const;
    };

    // A clothing component
    class Cloth
    {
        uint16 drawable;
        uint8 texture;
        uint8 palette;

    };

    // A dlc clothing component
    class DlcCloth
    {
        uint16 drawable;
        uint8 texture;
        uint8 palette;
        uint dlc;

    };

    // A prop component
    class Prop
    {
        uint16 drawable;
        uint8 texture;

    };

    // A dlc prop component
    class DlcProp
    {
        uint16 drawable;
        uint8 texture;
        uint dlc;

    };

    // Base object superclass for all alt:V base objects
    class BaseObject
    {
        uint8 type;
        bool valid;

        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
    };

    // World object superclass for all alt:V world objects
    class WorldObject : BaseObject
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;

        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
    };

    // Entity superclass for all alt:V entities
    class Entity : WorldObject
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        uint16 id;
        Hash model;
        Vector3 rot;
        bool visible;

        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        Player@ GetNetOwner() const;
        void SetNetOwner(Player@ player, bool disableMigration = false);
        bool HasSyncedMeta(const string&in key);
        bool GetSyncedMeta(const string&in key, ?&out outValue);
        void SetSyncedMeta(const string&in key, ?&in value);
        void DeleteSyncedMeta(const string&in key);
        bool HasStreamSyncedMeta(const string&in key);
        bool GetStreamSyncedMeta(const string&in key, ?&out outValue);
        void SetStreamSyncedMeta(const string&in key, ?&in value);
        void DeleteStreamSyncedMeta(const string&in key);
        alt::Player@ opCast() const;
        alt::Vehicle@ opCast() const;
    };

    // alt:V Player Entity
    class Player : Entity
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        uint16 id;
        Hash model;
        Vector3 rot;
        bool visible;
        string name;
        uint health;
        uint maxHealth;
        uint armour;
        uint maxArmour;
        uint weapon;
        array<uint>@ weaponComponents;
        uint8 weaponTint;
        bool dead;
        bool jumping;
        bool inRagdoll;
        bool aiming;
        bool shooting;
        bool reloading;
        bool connected;
        float moveSpeed;
        Vector3 aimPos;
        Vector3 headRot;
        bool inVehicle;
        Vehicle@ vehicle;
        uint8 seat;
        Entity@ entityAimingAt;
        Vector3 entityAimOffset;
        bool flashlightActive;
        uint ping;
        string ip;
        uint64 socialId;
        uint64 hwid;
        uint64 hwidEx;
        string authToken;

        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        Player@ GetNetOwner() const;
        void SetNetOwner(Player@ player, bool disableMigration = false);
        bool HasSyncedMeta(const string&in key);
        bool GetSyncedMeta(const string&in key, ?&out outValue);
        void SetSyncedMeta(const string&in key, ?&in value);
        void DeleteSyncedMeta(const string&in key);
        bool HasStreamSyncedMeta(const string&in key);
        bool GetStreamSyncedMeta(const string&in key, ?&out outValue);
        void SetStreamSyncedMeta(const string&in key, ?&in value);
        void DeleteStreamSyncedMeta(const string&in key);
        void Spawn(float x, float y, float z, uint delay = 0);
        void Spawn(int x, int y, int z, uint delay = 0);
        void Spawn(Vector3 pos, uint delay = 0);
        void Despawn();
        bool HasWeaponComponent(uint weapon, uint component);
        uint GetWeaponTint(uint weapon);
        void SetWeaponTint(uint weapon, uint8 tint);
        void AddWeaponComponent(uint weapon, uint component);
        void RemoveWeaponComponent(uint weapon, uint component);
        void GiveWeapon(uint weapon, int ammo, bool equipNow = false);
        bool RemoveWeapon(uint weapon);
        void RemoveAllWeapons();
        void ClearBloodDamage();
        void SetDateTime(uint8 day, uint8 month, uint8 year, uint8 hour, uint8 minute, uint8 second);
        void SetWeather(uint8 weather);
        void Kick(const string&in reason = "Kicked");
        bool IsEntityInStreamRange(Entity@ entity);
        void Emit(const string&in event, ...);
        Cloth GetClothes(uint8 component);
        DlcCloth GetDlcClothes(uint8 component);
        void SetClothes(uint8 component, uint16 drawable, uint8 texture, uint8 palette = 2);
        void SetDlcClothes(uint8 component, uint16 drawable, uint8 texture, uint8 palette, uint dlc);
        Prop GetProps(uint8 component);
        DlcProp GetDlcProps(uint8 component);
        void SetProps(uint8 component, uint16 drawable, uint8 texture, uint8 palette = 2);
        void SetDlcProps(uint8 component, uint16 drawable, uint8 texture, uint8 palette, uint dlc);
        string opImplConv() const;
    };

    // alt:V Vehicle Entity
    class Vehicle : Entity
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        uint16 id;
        Hash model;
        Vector3 rot;
        bool visible;
        Player@ driver;
        bool destroyed;
        uint8 modKitsCount;
        uint8 modKit;
        bool hasCustomPrimaryColor;
        uint8 primaryColor;
        RGBA primaryColorRGB;
        bool hasCustomSecondaryColor;
        uint8 secondaryColor;
        RGBA secondaryColorRGB;
        uint8 pearlColor;
        uint8 wheelColor;
        uint8 interiorColor;
        uint8 dashboardColor;
        bool hasCustomTireSmokeColor;
        RGBA tireSmokeColor;
        uint8 wheelType;
        uint8 wheelVariation;
        uint8 rearWheelVariation;
        bool customTires;
        uint8 specialDarkness;
        uint numberplateIndex;
        string numberplateText;
        uint8 windowTint;
        uint8 dirtLevel;
        bool neonActive;
        RGBA neonColor;
        uint8 livery;
        uint8 roofLivery;
        bool engineOn;
        bool handbrakeActive;
        uint8 headlightColor;
        uint radioStation;
        bool sirenActive;
        uint8 lockState;
        bool daylightOn;
        bool nightlightOn;
        uint8 roofState;
        bool flamethrowerActive;
        float lightsMultiplier;
        uint engineHealth;
        uint petrolTankHealth;
        uint8 wheelsCount;
        uint8 repairsCount;
        uint bodyHealth;
        uint additionalBodyHealth;
        bool armoredWindows;
        bool manualEngineControl;
        Vehicle@ attached;
        Vehicle@ attachedTo;

        Vehicle(Hash model, Vector3&in pos, Vector3&in rot);
        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        Player@ GetNetOwner() const;
        void SetNetOwner(Player@ player, bool disableMigration = false);
        bool HasSyncedMeta(const string&in key);
        bool GetSyncedMeta(const string&in key, ?&out outValue);
        void SetSyncedMeta(const string&in key, ?&in value);
        void DeleteSyncedMeta(const string&in key);
        bool HasStreamSyncedMeta(const string&in key);
        bool GetStreamSyncedMeta(const string&in key, ?&out outValue);
        void SetStreamSyncedMeta(const string&in key, ?&in value);
        void DeleteStreamSyncedMeta(const string&in key);
        string opImplConv() const;
        uint8 GetMod(uint8 category);
        bool SetMod(uint8 category, uint8 mod);
        uint8 GetModsCount(uint8 category);
        bool IsExtraOn(uint8 extra);
        void SetExtraOn(uint8 extra, bool toggle);
        void GetActiveNeon(bool&out left, bool&out right, bool&out front, bool&out back);
        void SetActiveNeon(bool left, bool right, bool front, bool back);
        uint8 GetDoorState(uint8 door);
        void SetDoorState(uint8 door, uint8 state);
        bool IsWindowOpened(uint8 window);
        void SetWindowOpened(uint8 window, bool opened);
        bool IsWheelBurst(uint8 wheel);
        void SetWheelBurst(uint8 wheel, bool state);
        bool DoesWheelHaveTire(uint8 wheel);
        void SetWheelHasTire(uint8 wheel, bool state);
        bool IsWheelDetached(uint8 wheel);
        void SetWheelDetached(uint8 wheel, bool state);
        bool IsWheelOnFire(uint8 wheel);
        void SetWheelOnFire(uint8 wheel, bool state);
        float GetWheelHealth(uint8 wheel);
        void SetWheelHealth(uint8 wheel, float health);
        uint8 GetPartDamageLevel(uint8 part);
        void SetPartDamageLevel(uint8 part, uint8 level);
        uint8 GetPartBulletholes(uint8 part);
        void SetPartBulletholes(uint8 part, uint8 amount);
        bool IsLightDamaged(uint8 light);
        void SetLightDamaged(uint8 light, bool state);
        bool IsWindowDamaged(uint8 window);
        void SetWindowDamaged(uint8 window, bool state);
        bool IsSpecialLightDamaged(uint8 light);
        void SetSpecialLightDamaged(uint8 light, bool state);
        float GetArmoredWindowHealth(uint8 window);
        float SetArmoredWindowHealth(uint8 window, float health);
        uint8 GetArmoredWindowBulletholes(uint8 window);
        void SetArmoredWindowBulletholes(uint8 window, uint8 amount);
        uint8 GetBumperDamageLevel(uint8 bumper);
        void SetBumperDamageLevel(uint8 bumper, uint8 level);
        void Repair();
    };

    // alt:V Voice Channel
    class VoiceChannel : BaseObject
    {
        uint8 type;
        bool valid;
        bool spatial;
        float maxDistance;

        VoiceChannel(bool isSpatial = false, float maxDistance = 0);
        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        string opImplConv() const;
        bool HasPlayer(Player@ player);
        void AddPlayer(Player@ player);
        void RemovePlayer(Player@ player);
        bool IsPlayerMuted(Player@ player);
        void MutePlayer(Player@ player);
        void UnmutePlayer(Player@ player);
    };

    // alt:V Generic ColShape
    class ColShape : WorldObject
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        int8 shapeType;
        bool playersOnly;

        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        string opImplConv() const;
        bool IsPointIn(Vector3 point);
        bool IsEntityIn(Entity@ entity);
    };

    // alt:V ColShape sphere
    class ColShapeSphere : ColShape
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        int8 shapeType;
        bool playersOnly;

        ColShapeSphere(Vector3 pos, float radius);
        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        string opImplConv() const;
        bool IsPointIn(Vector3 point);
        bool IsEntityIn(Entity@ entity);
    };

    // alt:V ColShape cylinder
    class ColShapeCylinder : ColShape
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        int8 shapeType;
        bool playersOnly;

        ColShapeCylinder(Vector3 pos, float radius, float height);
        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        string opImplConv() const;
        bool IsPointIn(Vector3 point);
        bool IsEntityIn(Entity@ entity);
    };

    // alt:V ColShape circle
    class ColShapeCircle : ColShape
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        int8 shapeType;
        bool playersOnly;

        ColShapeCircle(Vector3 pos, float radius);
        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        string opImplConv() const;
        bool IsPointIn(Vector3 point);
        bool IsEntityIn(Entity@ entity);
    };

    // alt:V ColShape cuboid
    class ColShapeCuboid : ColShape
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        int8 shapeType;
        bool playersOnly;

        ColShapeCuboid(Vector3 pos, Vector3 pos2);
        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        string opImplConv() const;
        bool IsPointIn(Vector3 point);
        bool IsEntityIn(Entity@ entity);
    };

    // alt:V ColShape rectangle
    class ColShapeRect : ColShape
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        int8 shapeType;
        bool playersOnly;

        ColShapeRect(Vector2 pos, Vector2 pos2, float z);
        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        string opImplConv() const;
        bool IsPointIn(Vector3 point);
        bool IsEntityIn(Entity@ entity);
    };

    // alt:V ColShape checkpoint
    class Checkpoint : ColShape
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        int8 shapeType;
        bool playersOnly;

        Checkpoint(uint8 type, Vector3 pos, float radius, float height, RGBA color);
        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        string opImplConv() const;
        bool IsPointIn(Vector3 point);
        bool IsEntityIn(Entity@ entity);
    };

    // alt:V Blip
    class Blip : WorldObject
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        uint16 blipType;
        bool global;
        Player@ target;
        bool attached;
        Entity@ attachedTo;

        Blip(Player@ target, uint16 type, Vector3 pos);
        Blip(Player@ target, uint16 type, Entity@ attachTo);
        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
    };
}
