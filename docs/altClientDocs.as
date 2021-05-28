// Generated on Fri May 28 23:55:46 2021

namespace altClient
{
    // ********** Funcdefs **********

    // Callback used for timers
    funcdef void TimerCallback();

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

    string version;

    string branch;

    uint sdkVersion;

    bool debugMode;

    string resourceMain;

    string resourceName;

    // ********** Functions **********

    // Hashes the given string using the joaat algorithm
    uint Hash(const string&in value);

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

    // Gets the exports of the specified resource
    dictionary@ GetResourceExports(const string&in name);

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

    // Emits a local event (Max 32 args)
    void Emit(const string&in event, ...);

    // Returns whether the specified meta key exists
    bool HasMeta(const string&in key);

    // Sets the specified meta key to the specified value
    bool GetMeta(const string&in key, ?&out outValue);

    // Returns whether the specified synced meta key exists
    bool HasSyncedMeta(const string&in key);

    // Sets the specified synced meta key to the specified value
    bool GetSyncedMeta(const string&in key, ?&out outValue);

    // ********** Objects **********

    // Three-dimensional vector
    class Vector3
    {
        float x;
        float y;
        float z;

        Vector3(float x, float y, float z);
        Vector3(int x, int y, int z);
        float Length();
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
        string opImplConv() const;
    };

    // Two-dimensional vector
    class Vector2
    {
        float x;
        float y;

        Vector2(float x, float y);
        Vector2(int x, int y);
        float Length();
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
        string opImplConv() const;
    };

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

    // Benchmarking utility
    class Benchmark
    {
        bool ended;

        Benchmark(const string&in name = "Unnamed_Benchmark", bool autoStart = true, bool showLog = true);
        void Start();
        double End();
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
        uint model;
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
        Player@+ GetNetOwner() const;
        bool HasSyncedMeta(const string&in key);
        bool GetSyncedMeta(const string&in key, ?&out outValue);
        bool HasStreamSyncedMeta(const string&in key);
        bool GetStreamSyncedMeta(const string&in key, ?&out outValue);
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
        uint model;
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
        float moveSpeed;
        Vector3 aimPos;
        Vector3 headRot;
        bool inVehicle;
        Vehicle@ vehicle;
        uint8 seat;
        Entity@ entityAimingAt;
        Vector3 entityAimOffset;
        bool flashlightActive;

        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        Player@+ GetNetOwner() const;
        bool HasSyncedMeta(const string&in key);
        bool GetSyncedMeta(const string&in key, ?&out outValue);
        bool HasStreamSyncedMeta(const string&in key);
        bool GetStreamSyncedMeta(const string&in key, ?&out outValue);
        string opImplConv() const;
        bool HasWeaponComponent(uint weapon, uint component);
        uint GetWeaponTint(uint weapon);
    };

    // alt:V Vehicle Entity
    class Vehicle : Entity
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        uint16 id;
        uint model;
        Vector3 rot;
        bool visible;
        Player@+ driver;
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

        bool HasMeta(const string&in key);
        bool GetMeta(const string&in key, ?&out outValue);
        void SetMeta(const string&in key, ?&in value);
        void DeleteMeta(const string&in key);
        bool HasData(const string&in key);
        bool GetData(const string&in key, ?&out outValue);
        void SetData(const string&in key, ?&in value);
        void DeleteData(const string&in key);
        void Destroy();
        Player@+ GetNetOwner() const;
        bool HasSyncedMeta(const string&in key);
        bool GetSyncedMeta(const string&in key, ?&out outValue);
        bool HasStreamSyncedMeta(const string&in key);
        bool GetStreamSyncedMeta(const string&in key, ?&out outValue);
        string opImplConv() const;
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
        Player@+ target;
        bool attached;
        Entity@+ attachedTo;

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
