#pragma once
#include "System.hpp"

class Arlia_type {
private:
	struct Type {
		std::string Identifier;
		size_t ByteSize;
		std::vector<class Arlia_field> Fields;
		std::vector<class Arlia_method> Methods;
		std::vector<class Arlia_upon> Operators;
	};
public:
	Type type;
	void SetType(std::string Identifier, size_t ByteSize, std::vector<class Arlia_field> Fields, std::vector<class Arlia_method> Methods, std::vector<class Arlia_upon> Operators) {
		type.Identifier = Identifier;
		type.ByteSize = ByteSize;
		type.Fields = Fields;
		type.Methods = Methods;
		type.Operators = Operators;
	}
};
class Arlia_field {
private:
	struct Field {
		std::string Identifier;
		Arlia_type Type;
		bool IsConst;
		bool IsVisible;
		std::string Adress;
		std::string DirectValue;
	};
public:
	Field field;
	void SetField(std::string Identifier, Arlia_type Type, bool IsConst, bool IsVisible, std::string Adress, std::string DirectValue = "") {
		field.Identifier = Identifier;
		field.Type = Type;
		field.IsConst = IsConst;
		field.IsVisible = IsVisible;
		field.Adress = Adress;
		field.DirectValue = DirectValue;
	}
};
class Arlia_method {
private:
	struct Method {
		std::string Identifier;
		std::vector<Arlia_field> Parameters;
		Arlia_type Type;
		bool IsVisible;
	};
public:
	Method method;
	void SetMethod(std::string Identifier, std::vector<Arlia_field> Parameters, Arlia_type Type, bool IsVisible) {
		method.Identifier = Identifier;
		method.Parameters = Parameters;
		method.Type = Type;
		method.IsVisible = IsVisible;
	}
};
class Arlia_upon{
private:

public:

};