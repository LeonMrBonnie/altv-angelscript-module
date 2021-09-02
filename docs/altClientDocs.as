// Generated on Fri Jul 16 02:40:48 2021

namespace altClient
{
    // ********** Funcdefs **********

    // Callback used for timers
    funcdef void TimerCallback();

    // Callback used for screenshot methods
    funcdef void ScreenshotCallback(const string&in base64Image);

    // ********** Typedefs **********

    typedef uint32 Hash;

    typedef int32 ScriptID;

    // ********** Enums **********

    // A alt:V permission
    enum Permission
    {
        None = 0,
        ScreenCapture = 1,
        WebRTC = 2,
        All = 3,
    };

    // A alt:V permission state
    enum PermissionState
    {
        Allowed = 0,
        Denied = 1,
        Unspecified = 2,
        Failed = 3,
    };

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

    bool sandboxMode;

    bool gameControlsEnabled;

    bool cursorShown;

    bool voiceInputMuted;

    bool voiceActivationEnabled;

    uint voiceActivationKey;

    string licenseHash;

    string locale;

    bool streamerMode;

    bool menuOpen;

    bool consoleOpen;

    bool isGameFocused;

    LocalPlayer@+ localPlayer;

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

    // Gets the current discord state
    DiscordData GetDiscordData();

    // Sets the specified stat to the specified value
    void SetStat(const string&in stat, ?&in value);

    // Gets the specified stat value
    void GetStat(const string&in stat, ?&out value);

    // Resets the specified stat value
    void ResetStat(const string&in stat);

    // Takes a screenshot of the alt:V window
    void TakeScreenshot(ScreenshotCallback@ callback);

    // Takes a screenshot of the alt:V window and emits the result to the specified event
    void TakeScreenshot(const string&in eventName);

    // Takes a screenshot of the GTA V window
    void TakeScreenshotGameOnly(ScreenshotCallback@ callback);

    // Takes a screenshot of the GTA V window and emits the result to the specified event
    void TakeScreenshotGameOnly(const string&in eventName);

    // Emits an event to the server (Max 32 args)
    void EmitServer(const string&in event, ...);

    // Gets the state of the specified key
    KeyState GetKeyState(uint key);

    // Toggles whether the game receives control inputs
    void SetGameControlsEnabled(bool state);

    // Adds a gxt text
    void AddGxtText(const string&in gxt, const string&in text);

    // Adds a gxt text
    void AddGxtText(Hash hash, const string&in text);

    // Removes a gxt text
    void RemoveGxtText(const string&in gxt);

    // Removes a gxt text
    void RemoveGxtText(Hash hash);

    // Gets a gxt text value
    bool GetGxtText(const string&in gxt);

    // Gets a gxt text value
    void GetGxtText(Hash hash);

    // Shows or hides the cursor, returns whether the operation succeeded or not
    bool ShowCursor(bool state);

    // Sets the specified config flag, returns whether the operation succeded or not
    bool SetConfigFlag(const string&in flag, bool state);

    // Gets the value of the specified config flag
    bool GetConfigFlag(const string&in flag);

    // Returns whether the given config flag exists
    bool DoesConfigFlagExist(const string&in flag);

    // Mutes/Unmutes the voice input in the alt:V voice chat
    void SetVoiceInputMuted(bool state);

    // Toggles whether the voice controls are enabled or not
    void SetVoiceControlsEnabled(bool state);

    // Gets the entity with the specified script id, or null if not found
    Entity@ GetEntityByScriptId(int scriptId);

    // Checks if the given model with the given texture exists
    bool DoesTextureExistInArchetype(uint model, const string&in texture);

    // Checks if the given model with the given texture exists
    bool DoesTextureExistInArchetype(const string&in model, const string&in texture);

    // Loads the given ipl
    void RequestIpl(const string&in ipl);

    // Unloads the given ipl
    void RemoveIpl(const string&in ipl);

    // Starts a movie method on the minimap
    bool BeginScaleformMovieMethodMinimap(const string&in method);

    // Sets the time required in milliseconds for one game minute to pass
    void SetMsPerGameMinute(int amount);

    // Gets the time required in milliseconds for one game minute to pass
    int GetMsPerGameMinute();

    // Sets the current weather cycle
    void SetWeatherCycle(array<uint8>@ weathers, array<uint8>@ multipliers);

    // Toggles whether the weather sync is enabled
    void SetWeatherSyncActive(bool state);

    // Toggles whether the cam is frozen
    void SetCamFrozen(bool state);

    // Gets the state of the given permission
    PermissionState GetPermissionState(Permission permission);

    // Sets the angular velocity of the specified entity
    void SetAngularVelocity(ScriptID entity, const Vector3&in velocity);

    // Loads the given model into memory
    void LoadModel(Hash hash, bool async = false);

    // Loads the YTYP at the given path into memory
    bool LoadYtyp(const string&in path);

    // Unloads the YTYP at the given path out of memory
    bool UnloadYtyp(const string&in path);

    // Gets the base64 encoded image of the given headshot
    string GetHeadshotBase64(uint8 id);

    // ********** Objects **********

    // Discord data
    class DiscordData
    {
        bool ready;
        string userID;
        string username;
        string discriminator;
        string avatar;

    };

    // The current state of a keyboard key
    class KeyState
    {
        bool down;
        bool toggled;

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
        ScriptID scriptID;

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
        Hash model;
        Vector3 rot;
        bool visible;
        ScriptID scriptID;
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
        bool isTalking;
        float micLevel;
        float spatialVolume;
        float nonSpatialVolume;

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
        bool HasSyncedMeta(const string&in key);
        bool GetSyncedMeta(const string&in key, ?&out outValue);
        bool HasStreamSyncedMeta(const string&in key);
        bool GetStreamSyncedMeta(const string&in key, ?&out outValue);
        bool HasWeaponComponent(uint weapon, uint component);
        uint GetWeaponTint(uint weapon);
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
        ScriptID scriptID;
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
        float wheelSpeed;
        uint16 gear;
        uint16 maxGear;
        float rpm;
        Vector3 speedVector;
        uint8 lightsIndicator;
        bool handlingModified;

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
        bool HasSyncedMeta(const string&in key);
        bool GetSyncedMeta(const string&in key, ?&out outValue);
        bool HasStreamSyncedMeta(const string&in key);
        bool GetStreamSyncedMeta(const string&in key, ?&out outValue);
        string opImplConv() const;
        void ResetHandling();
        void ReplaceHandling();
    };

    // alt:V Local Player instance
    class LocalPlayer : Player
    {
        uint8 type;
        bool valid;
        Vector3 pos;
        int dimension;
        uint16 id;
        Hash model;
        Vector3 rot;
        bool visible;
        ScriptID scriptID;
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
        bool isTalking;
        float micLevel;
        float spatialVolume;
        float nonSpatialVolume;
        uint16 currentAmmo;

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
        bool HasSyncedMeta(const string&in key);
        bool GetSyncedMeta(const string&in key, ?&out outValue);
        bool HasStreamSyncedMeta(const string&in key);
        bool GetStreamSyncedMeta(const string&in key, ?&out outValue);
        bool HasWeaponComponent(uint weapon, uint component);
        uint GetWeaponTint(uint weapon);
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
        Player@ target;
        bool attached;
        Entity@ attachedTo;

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
