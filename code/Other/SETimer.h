#pragma once

#include "SEString.h"

namespace io
{
    //! Types of time.
    enum ETimeTypes
    {
        TIME_SECOND = 0,
        TIME_MINUTE,
        TIME_HOUR,
        TIME_DAY,
        TIME_MONTH,
        TIME_YEAR
    };

#ifdef SE_PLATFORM_WINDOWS
    struct SFrequenceQuery;
#endif


    /**
    Performance time query macro.
    \code
    // Start performance query.
    PERFORMANCE_QUERY_START(MyTimer)

    // Do some stuff for which you want to make a performance query.
    // ...

    // Print performance query result (in microseconds).
    PERFORMANCE_QUERY_PRINT("Duration (in microseconds): ", MyTimer)
    \endcode
    */
#define PERFORMANCE_QUERY_START(t)  \
    io::Timer t(true);              \
    t.resetClockCounter();

#define PERFORMANCE_QUERY_RESULT(t) \
    t.getElapsedMicroseconds()

#define PERFORMANCE_QUERY_PRINT(msg, t) \
    io::Log::message(msg + io::stringc(PERFORMANCE_QUERY_RESULT(t)));


    /**
    The Timer class can be used as stop watch or just to have simple time access.
    \since Version 3.2
    */
    class Timer
    {

    public:

        Timer(u64 Duration = 0);
        Timer(bool UseFrequenceQuery);
        virtual ~Timer();

        /* === Functions === */

        //! Starts the stop watch with the given duration.
        void start(u64 Duration);
        //! Stops the stop watch.
        void stop();
        //! Pauses the stop watch.
        void pause(bool isPaused = true);
        //! Resets the stop watch and uses the same duration as before.
        void reset();

        //! Returns true if the timer has finished.
        bool finish();

        /**
        Returns the elapsed microseconds since the last reset or creation time.
        \note This can only be used if this timer was created with a frequence query.
        \see resetClockCounter
        */
        u64 getElapsedMicroseconds();

        /**
        Resets the clock counter which is used get the elapsed microseconds.
        \see getElapsedMicroseconds
        */
        void resetClockCounter();

        /**
        Returns the statistic count of frames per seconds.
        \note This can only be used if this timer was created with a frequence query.
        Otherwise the function will always return 60.
        \code
        io::Timer timer(true);
        // ...
        f64 fps = timer.getCurrentFPS();
        \endcode
        \see getElapsedMicroseconds
        */
        f64 getCurrentFPS();

        //! Returns true if the timer was paused.
        inline bool paused() const
        {
            return TimeOut_ != 0;
        }

        /* === Static functions === */

        /**
        Returns the count of elapsed microseconds since your computer started.
        \note This is actually only supported for Linux systems. For Windows you can use "getElapsedMicroseconds".
        \see getElapsedMicroseconds
        */
        static u64 microsecs();

        //! Returns the count of elapsed milliseconds since your computer started.
        static u64 millisecs();

        //! Returns the count of elapsed seconds since your computer started.
        static u64 secs();

        //! Returns the specified time value.
        static u32 getTime(const ETimeTypes Type);

        //! Returns current time as a string in the form "DD/MM/YYYY HH:MM:SS" (e.g. "03/01/2008 15:23:46").
        static io::stringc getTime();

        /**
        Returns the given seconds as string (e.g. 1325 -> "00:22:05").
        \since Version 3.3
        \see millisecs
        */
        static io::stringc secsAsString(u64 Seconds);

        /**
        Returns the statistic count of frames per seconds.
        \param UpdateFrameRate: Specifies the count of frames which need to elapse before the new
        FPS value will be computed (e.g. 5 is a good value).
        \return Frames per second as double precision floating point (statistic value).
        */
        static f64 getFPS();

        /**
        Returns the count of frame since the elapsed second.
        \param Duration: Specifies the duration after which the elapsed frames will be updated.
        \note Call this function only once in each frame!
        */
        static u32 getElapsedFrames(u64 Duration = 1000);

        /**
        Enables the global speed adjustment. By default enabled.
        \see getGlobalSpeed
        */
        static void setGlobalSpeedEnable(bool Enable);
        //! Returbs the global speed adjustment. By default enabled.
        static bool getGlobalSpeedEnable();

        /**
        Sets the global speed multiplier. By default 1.0. If you want that all animations etc.
        run faster, increase this value.
        \see getGlobalSpeed
        */
        static void setGlobalSpeedMultiplier(f32 Factor);
        //! Returbs the global speed multiplier. By default 1.0.
        static f32 getGlobalSpeedMultiplier();

        /**
        Returns the global speed. When the global FPS is 60.0 this value is 1.0.
        If the global FPS value is greater then the return value is smaller.
        This is used that animations and other scene movements look always the same,
        regardless of FPS.
        \see getFPS
        */
        static f32 getGlobalSpeed();

        /**
        Converts the given duration when the 'global speed multiplier' is enabled.
        \return Converted duration. When the multiplier is 2.0 the return value of 1000 milliseconds is 500.
        When the multiplier is 0.25 the return value of 1000 milliseconds is 4000.
        */
        static u64 convertDuration(u64 Duration);

        //! Waits for the specified time.
        static void sleep(u32 Milliseconds);

        //! Waits for 1 millisecond to give other processes time to run. This can be used in multi-threading.
        static inline void yield()
        {
            sleep(1);
        }

    protected:

        /* === Members === */

        u64 StartTime_, EndTime_, TimeOut_, Duration_;

    private:

        /* === Functions === */

        void createFrequenceQuery();

        static void updateGlobalFPSCounter();

        /* === Members === */

#ifdef SE_PLATFORM_WINDOWS
        SFrequenceQuery* FreqQuery_;
#endif

        static f64 GlobalFPS_;              //!< Global base FPS counter variable.

        static bool GlobalSpeedEnabled_;
        static f32 GlobalSpeedMultiplier_;
        static f32 GlobalSpeed_;            //!< Global speed variable (GlobalSpeed := (60.0 * GlobalSpeedMultiplier) / GlobalFPS).

    };


} // /namespace io