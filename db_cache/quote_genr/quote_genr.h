#pragma once

#include <cstdlib>
#include <fstream>
#include <vector>



const std::vector<std::string> quote_sources = {
	"Yoda",
	"Obiwan",
	"Ashoka",
	"Vader",
	"Palpatine",
	"Qui Gon Jinn",
	"Padme Amidala",
	"C3P0",
	"Han Solo",
	"Leia Organa",
	"Chewbacca",
	"Lando Calrissian",
	"Ezra Bridger",
	"Hera Syndulla",
	"Kanan Jarrus",
	"Sabine Wren",
	"Rex",
	"Zeb Orrelios",
	"Luke Skywalker",
	"Count Dooku"
};



void genr_io(std::ofstream& infile, std::ofstream& outfile, size_t lines);

std::string random_str(const size_t len);
size_t random_num(const size_t range);
size_t random_num(const size_t lo, const size_t hi);