#pragma once
#include <exception>

class NullPointerException : public std::exception {
	const char* what() const throw() {
		return "Give me an object to work on! I can't do anything with the  v o i d";
	}
};

class ArgumentException : public std::exception {
	const char* what() const throw() {
		return "Invalid argument passed to a function";
	}
};

class InvalidAudioFileException : public std::exception {
	const char* what() const throw() {
		return "File could not be loaded.\n Channel count and/or samplerate are not matching";
	}
};