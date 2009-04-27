#include <string>
#include <gio/gio.h>
#include <stdlib.h>

class fsProvider {
	private:
		GError *errorHandler;
		static fsProvider* pInstance;
		char *userHome;
		GFile *ROOT;
		GFile *HOME;
	public:
		static fsProvider* Instance();
		GFileEnumerator* getChildren(GFile* trunk);
		GFile* getRoot();
		GFile* getHome();
		bool isHidden(GFile*);
		const char* getName(GFile*);
		const char* getPath(GFile*);
		bool isHidden(GFileInfo*);
		const char* getName(GFileInfo*);
		const char* getPath(GFileInfo*);
	protected:
		fsProvider();
		fsProvider(const fsProvider&);
		fsProvider& operator= (const fsProvider&);
};
