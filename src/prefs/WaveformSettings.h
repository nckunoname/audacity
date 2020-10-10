/**********************************************************************

Audacity: A Digital Audio Editor

WaveformSettings.h

Paul Licameli

**********************************************************************/

#ifndef __AUDACITY_WAVEFORM_SETTINGS__
#define __AUDACITY_WAVEFORM_SETTINGS__

#include "ClientData.h" // to inherit
#include "Prefs.h"

class wxRect;

class EnumValueSymbols;
class WaveTrack;

class AUDACITY_DLL_API WaveformSettings
   : public PrefsListener
   , public ClientData::Cloneable< ClientData::UniquePtr >
{
public:

   //! Create waveform settings for the track on demand
   static WaveformSettings &Get( WaveTrack &track );

   //! @copydoc Get
   static const WaveformSettings &Get( const WaveTrack &track );

   //! Guarantee independence of settings, then assign
   static void Set(
      WaveTrack &track, std::unique_ptr<WaveformSettings> pSettings );

   // Singleton for settings that are not per-track
   class AUDACITY_DLL_API Globals
   {
   public:
      static Globals &Get();
      void SavePrefs();

   private:
      Globals();
      void LoadPrefs();
   };

   static WaveformSettings &defaults();

   WaveformSettings();
   WaveformSettings(const WaveformSettings &other);
   WaveformSettings& operator= (const WaveformSettings &other);
   ~WaveformSettings() override;

   PointerType Clone() const override;

   bool IsDefault() const
   {
      return this == &defaults();
   }

   bool Validate(bool quiet);
   void LoadPrefs();
   void SavePrefs();
   void Update();

   void UpdatePrefs() override;

   void ConvertToEnumeratedDBRange();
   void ConvertToActualDBRange();
   void NextLowerDBRange();
   void NextHigherDBRange();

   typedef int ScaleType;
   enum ScaleTypeValues : int {
      stLinear,
      stLogarithmic,

      stNumScaleTypes,
   };

   static const EnumValueSymbols &GetScaleNames();

   ScaleType scaleType;
   int dBRange;

   // Convenience
   bool isLinear() const { return stLinear == scaleType; }
};

class AUDACITY_DLL_API WaveformScale
   : public ClientData::Cloneable< ClientData::UniquePtr >
{
public:

   static WaveformScale &Get( WaveTrack &track );

   //! @copydoc Get
   static const WaveformScale &Get( const WaveTrack &track );

   ~WaveformScale() override;
   PointerType Clone() const override;

   int ZeroLevelYCoordinate(wxRect rect) const;

   void GetDisplayBounds(float &min, float &max) const
   { min = mDisplayMin; max = mDisplayMax; }

   void SetDisplayBounds(float min, float max) const
   { mDisplayMin = min; mDisplayMax = max; }

   float GetLastScaleType() const { return mLastScaleType; }

   mutable float mDisplayMin = -1.0f, mDisplayMax = 1.0f;
   mutable int mLastScaleType = -1;
   mutable int mLastdBRange = -1;
};

#endif
