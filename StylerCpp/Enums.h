#pragma once
namespace Styler
{
	/// <summary>
	///     Note enum type is used to represent notes CS = C#, etc.
	/// </summary>

	enum class Chord
	{
		None,
		Drum,
		C,
		D,
		E,
		F,
		G,
		A,
		B,
		Cm,
		Dm,
		Em,
		Fm,
		Gm,
		Am,
		Bm
	};

	enum Note
	{
		C = 24,
		CS = 25,
		D = 26,
		DS = 27,
		E = 28,
		F = 29,
		FS = 30,
		G = 31,
		GS = 32,
		A = 33,
		AS = 34,
		B = 35
	};

	/// <summary>
	///     Enum representing the type of style's track.
	///     Intro tracks are tracks that are played once and then switched to a Main track.
	///     Main tracks are tracks that loop and are the main part of the style.
	///     Ending tracks are played once, and after they finish playing the style is stopped.
	///     Fill tracks are used for transitions between tracks.
	/// </summary>
	enum class TrackType
	{
		Intro,
		Fill,
		Main,
		Break,
		Ending
	};
}