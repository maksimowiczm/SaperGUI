#pragma once
#include <map>
#include <vector>

namespace konsola
{
	class Alfabet
	{
	public:
		constexpr static char SPACE = 32;
		constexpr static char BLOCK = -37;
		using Letter = std::vector<std::vector<bool>>;

		const std::map<char, Letter> alfabet{
			{
				NULL, {
					{true, true, true},
					{true, true, true},
					{true, true, true},
					{true, true, true},
					{true, true, true},
				}
			},
			{
				'a', {
					{false, true, true, false},
					{true, false, false, true},
					{true, false, false, true},
					{true, true, true, true},
					{true, false, false, true},
				}
			},
			{
				'b', {
					{true, true, true, false},
					{true, false, false, true},
					{true, true, true, false},
					{true, false, false, true},
					{true, true, true, false},
				}
			},
			{
				'c', {
					{true, true, true},
					{true, false, false},
					{true, false, false},
					{true, false, false},
					{true, true, true},
				}
			},
			{
				'd', {
					{true, true, false},
					{true, false, true},
					{true, false, true},
					{true, false, true},
					{true, true, false},
				}
			},
			{
				'e', {
					{true, true, true},
					{true, false, false},
					{true, true, true},
					{true, false, false},
					{true, true, true},
				}
			},
			{
				'g', {
					{true, true, true, true},
					{true, false, false, false},
					{true, false, true, true},
					{true, false, false, true},
					{true, true, true, true},
				}
			},
			{
				'i', {
					{true},
					{true},
					{true},
					{true},
					{true},
				}
			},
			{
				'j', {
					{false, false, true},
					{false, false, true},
					{false, false, true},
					{false, false, true},
					{true, true, true},
				}
			},
			{
				'k', {
					{true, false, false, true},
					{true, false, true, false},
					{true, true, false, false},
					{true, false, true, false},
					{true, false, false, true},
				}
			},
			{
				'l', {
					{true, false, false},
					{true, false, false},
					{true, false, false},
					{true, false, false},
					{true, true, true},
				}
			},
			{
				'm', {
					{true, false, false, false, true},
					{true, true, false, true, true},
					{true, false, true, false, true},
					{true, false, false, false, true},
					{true, false, false, false, true},
				}
			},
			{
				'n', {
					{true, false, false, false, true},
					{true, true, false, false, true},
					{true, false, true, false, true},
					{true, false, false, true, true},
					{true, false, false, false, true},
				}
			},
			{
				'o', {
					{true, true, true},
					{true, false, true},
					{true, false, true},
					{true, false, true},
					{true, true, true},
				}
			},
			{
				'p', {
					{true, true, false},
					{true, false, true},
					{true, true, false},
					{true, false, false},
					{true, false, false},
				}
			},
			{
				'r', {
					{true, true, false},
					{true, false, true},
					{true, true, false},
					{true, false, true},
					{true, false, true},
				}
			},
			{
				's',
				{
					{true, true, true},
					{true, false, false},
					{true, true, true},
					{false, false, true},
					{true, true, true},
				},
			},
			{
				't', {
					{true, true, true},
					{false, true, false},
					{false, true, false},
					{false, true, false},
					{false, true, false},
				}
			},
			{
				'u', {
					{true, false, true},
					{true, false, true},
					{true, false, true},
					{true, false, true},
					{true, true, true},
				}
			},
			{
				'v', {
					{true, false, true},
					{true, false, true},
					{true, false, true},
					{true, false, true},
					{false, true, false},
				}
			},
			{
				'w', {
					{true, false, true, false, true},
					{true, false, true, false, true},
					{true, false, true, false, true},
					{true, false, true, false, true},
					{true, true, true, true, true},
				}
			},
			{
				'y', {
					{true, false, true},
					{true, false, true},
					{true, true, true},
					{false, true, false},
					{false, true, false},
				}
			},
			{
				'z', {
					{true, true, true, true},
					{false, false, false, true},
					{false, false, true, false},
					{false, true, false, false},
					{true, true, true, true},
				}
			},
			{
				' ', {
					{false, false, false},
					{false, false, false},
					{false, false, false},
					{false, false, false},
					{false, false, false},
				}
			},
			{
				'0', {
					{false, true, false},
					{true, false, true},
					{true, false, true},
					{true, false, true},
					{false, true, false},
				}
			},
			{
				'1', {
					{false, true, false},
					{true, true, false},
					{false, true, false},
					{false, true, false},
					{true, true, true},
				}
			},
			{
				'2', {
					{false, true, false},
					{true, false, true},
					{false, false, true},
					{false, true, false},
					{true, true, true},
				}
			},
			{
				'3', {
					{true, true, false},
					{false, false, true},
					{false, true, false},
					{false, false, true},
					{true, true, false},
				}
			},
			{
				'4', {
					{true, false, true},
					{true, false, true},
					{true, true, true},
					{false, false, true},
					{false, false, true},
				}
			},
			{
				'5', {
					{true, true, true},
					{true, false, false},
					{true, true, false},
					{false, false, true},
					{true, true, false},
				}
			},
			{
				'6', {
					{true, true, true},
					{true, false, false},
					{true, true, true},
					{true, false, true},
					{true, true, true},
				}
			},
			{
				'7', {
					{true, true, true},
					{false, false, true},
					{false, false, true},
					{false, true, false},
					{true, false, false},
				}
			},
			{
				'8', {
					{true, true, true},
					{true, false, true},
					{true, true, true},
					{true, false, true},
					{true, true, true},
				}
			},
			{
				'9', {
					{true, true, true},
					{true, false, true},
					{true, true, true},
					{false, false, true},
					{true, true, true},
				}
			},
		};

		Letter at(char) const;
	};
}
