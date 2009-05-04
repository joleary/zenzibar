#include <gtk/gtk.h>

class libraryWidget {

	public:
		libraryWidget();
		GtkWidget *getLibraryWidget();
		static void selected(GtkTreeSelection*,libraryWidget*);
		void hideColumn();
		void addItem();
	private:
		GtkWidget *listWidget;
		GtkListStore *listStore;
		
/*	uint32_t 	item_id
	uint32_t 	parent_id
	uint32_t 	storage_id
	char * 	title
	char * 	artist
	char * 	composer
	char * 	genre
	char * 	album
	char * 	date
	char * 	filename
	uint16_t 	tracknumber
	uint32_t 	duration
	uint32_t 	samplerate
	uint16_t 	nochannels
	uint32_t 	wavecodec
	uint32_t 	bitrate
	uint16_t 	bitratetype
	uint16_t 	rating
	uint32_t 	usecount
	uint64_t 	filesize	*/
		
		enum {
			ID,
			PARENT_ID,
			STORAGE_ID,
			NAME,
			ARTIST,
			COMPOSER,
			GENRE,
			ALBUM,
			DATE,
			FILENAME,
			TRACK_NUM,
			DURATION,
			RATING,
			NUM_COL
		};
		GtkTreeViewColumn *name_column;
		GtkTreeViewColumn *artist_column;
		GtkTreeViewColumn *composer_column;
		GtkTreeViewColumn *genre_column;
		GtkTreeViewColumn *album_column;
		GtkTreeViewColumn *date_column;
		GtkTreeViewColumn *track_column;
		GtkTreeViewColumn *duration_column;
		GtkTreeViewColumn *rating_column;
		
		void setColumns();
};
