#include "fsProvider.h"
#include <iostream>
fsProvider* fsProvider::Instance() {
	if(fsProvider::pInstance==0) {
		fsProvider::pInstance = new fsProvider();
	}
	return fsProvider::pInstance;
}

fsProvider::fsProvider() {
	userHome = getenv("HOME");
	// create reference for users homedir
	this->HOME = g_file_new_for_path(userHome);
	// cretae reference for filesystem root
	this->ROOT = g_file_new_for_path("/");
}

GFileEnumerator* fsProvider::getChildren(GFile* trunk) {
	return g_file_enumerate_children(trunk,"standard::*",G_FILE_QUERY_INFO_NONE,NULL,&errorHandler);
}

GFile* fsProvider::getHome() {
	if(this->HOME==NULL) {
		char *userHome = getenv("HOME");
		this->HOME = g_file_new_for_path(userHome);
	} 
	return this->HOME;
}

GFile* fsProvider::getRoot() {
	if(this->ROOT==NULL) {
		this->ROOT = g_file_new_for_path("/");
	}
	return this->ROOT;
}

const char* fsProvider::getName(GFile* fhandle) {
	GFileInfo *finfo = g_file_info_new();
	finfo = g_file_query_info(fhandle,"standard::*",G_FILE_QUERY_INFO_NONE,NULL,NULL);
	return g_file_info_get_attribute_string(finfo,G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME);
}

const char* fsProvider::getName(GFileInfo* finfo) {
	return g_file_info_get_attribute_string(finfo,G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME);
}

const char* fsProvider::getPath(GFile* fhandle) {
	GFileInfo *finfo = g_file_info_new();
	finfo = g_file_query_info(fhandle,"standard::*",G_FILE_QUERY_INFO_NONE,NULL,NULL);
	return g_file_info_get_attribute_string(finfo,G_FILE_ATTRIBUTE_STANDARD_TARGET_URI);
}

const char* fsProvider::getPath(GFileInfo* finfo) {
	return g_file_info_get_attribute_string(finfo,G_FILE_ATTRIBUTE_STANDARD_TARGET_URI);
}

bool fsProvider::isHidden(GFile* fhandle) {
	GFileInfo *finfo = g_file_info_new();
	finfo = g_file_query_info(fhandle,"standard::*",G_FILE_QUERY_INFO_NONE,NULL,NULL);
	return g_file_info_get_attribute_boolean(finfo,G_FILE_ATTRIBUTE_STANDARD_IS_HIDDEN);
}

bool fsProvider::isHidden(GFileInfo* finfo) {
	return g_file_info_get_attribute_boolean(finfo,G_FILE_ATTRIBUTE_STANDARD_IS_HIDDEN);
}

fsProvider* fsProvider::pInstance=0;
