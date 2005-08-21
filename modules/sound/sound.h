#ifndef KADU_SOUND_H
#define KADU_SOUND_H

#include <qobject.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qstringlist.h>
#include <qmap.h>
#include <qthread.h>
#include <qmutex.h>
#include <qsemaphore.h>

#include "config_file.h"
#include "modules.h"
#include "gadu.h"
#include "userlist.h"
#include "misc.h"
#include "message_box.h"

/**
	Uog�lniony deskryptor urz�dzenia d�wi�kowego.
**/
typedef void* SoundDevice;

/**
**/
enum SoundDeviceType {RECORD_ONLY, PLAY_ONLY, PLAY_AND_RECORD};

/**
	To jest klasa u�ywana wewn�trznie przez klas� SoundManager
	i nie powiniene� mie� potrzeby jej u�ywania.
**/
class SamplePlayThread : public QObject, public QThread
{
	Q_OBJECT

	private:
		SoundDevice Device;
		const int16_t* Sample;
		int SampleLen;
		bool Stopped;
		QSemaphore PlayingSemaphore;
		QSemaphore SampleSemaphore;

	protected:
		virtual void run();
		virtual void customEvent(QCustomEvent* event);

	public:
		SamplePlayThread(SoundDevice device);
		void playSample(const int16_t* data, int length);
		void stop();

	signals:
		void samplePlayed(SoundDevice device);
};

/**
	To jest klasa u�ywana wewn�trznie przez klas� SoundManager
	i nie powiniene� mie� potrzeby jej u�ywania.
**/
class SampleRecordThread : public QObject, public QThread
{
	Q_OBJECT

	private:
		SoundDevice Device;
		int16_t* Sample;
		int SampleLen;
		bool Stopped;
		QSemaphore RecordingSemaphore;
		QSemaphore SampleSemaphore;

	protected:
		virtual void run();
		virtual void customEvent(QCustomEvent* event);

	public:
		SampleRecordThread(SoundDevice device);
		void recordSample(int16_t* data, int length);
		void stop();

	signals:
		void sampleRecorded(SoundDevice device);
};

/**
	To jest klasa u�ywana wewn�trznie przez klas� SoundManager
	i nie powiniene� mie� potrzeby jej u�ywania.
**/
class SndParams
{
	public:
		SndParams(QString fm, bool volCntrl=false, float vol=1);
		SndParams(const SndParams &p);
		SndParams();

		QString filename;
		bool volumeControl;
		float volume;
};

/**
	To jest klasa u�ywana wewn�trznie przez klas� SoundManager
	i nie powiniene� mie� potrzeby jej u�ywania.
**/
class SoundPlayThread : public QThread
{
	public:
		SoundPlayThread();
		~SoundPlayThread();
		void run();
		void tryPlay(const char *path, bool volCntrl=false, float volume=1.0);
		void endThread();

	private:
		static bool play(const char *path, bool volCntrl=false, float volume=1.0);
		QMutex mutex;
		QSemaphore *semaphore;
		bool end;
		QValueList<SndParams> list;
};

class SoundManager : public Themes
{
    Q_OBJECT
	private:
		friend class SamplePlayThread;
		friend class SampleRecordThread;
		QTime lastsoundtime;
		bool mute;
		QMap<SoundDevice, SamplePlayThread*> PlayingThreads;
		QMap<SoundDevice, SampleRecordThread*> RecordingThreads;
		SoundPlayThread *play_thread;

		int simple_player_count;
		virtual void connectNotify(const char *signal);
		virtual void disconnectNotify(const char *signal);

	private slots:
		void newChat(const QString &protocolName, UserListElements senders, const QString &msg, time_t t);
		void newMessage(const QString &protocolName, UserListElements senders, const QString &msg, time_t t, bool &grab);
		void connectionError(const QString &protocolName, const QString &message);
		void userChangedStatusToAvailable(const QString &protocolName, UserListElement);
		void userChangedStatusToBusy(const QString &protocolName, UserListElement);
		void userChangedStatusToInvisible(const QString &protocolName, UserListElement);
		void userChangedStatusToNotAvailable(const QString &protocolName, UserListElement);

		/* from i ule s� ignorowane, message wskazuje na plik z d�wi�kiem do odtworzenia
		 * je�eli message==QString::null, to odtwarzany jest standardowy d�wi�k dla tego typu
		 * je�eli mapa jest!=NULL brane s� z niej nast�puj�ce warto�ci:
		 *		"Force"           - bool (wymuszenie odtwarzania mimo wyciszenia)
		 */
		void message(const QString &from, const QString &message, const QMap<QString, QVariant> *parameters, const UserListElement *ule);

	public slots:
		void play(const QString &path, bool force=false);
		void play(const QString &path, bool volCntrl, double vol);
		void setMute(const bool& enable);

	public:

		SoundManager(const QString& name, const QString& configname);
		~SoundManager();
		bool isMuted() const;
		int timeAfterLastSound() const;
		/**
			Otwiera urz�dzenie d�wi�kowe do operacji
			odtwarzania i nagrywania sampli.
			Niekt�re implementacje pozwalaj� na otwarcie
			wielu niezale�nie dzia�aj�cych "po��cze�"
			z urz�dzeniami. Wystarczy wtedy kilkukrotnie
			wywo�a� t� metod�.
			Emituje sygna� openDeviceImpl() w celu
			przekazania ��dania do konkrentego modu�u
			d�wi�kowego.
			@param type
			@param sample_rate sample rate - np. 8000 lub 48000
			@param channels ilo�� kana��w: 1 - mono, 2 - stereo
			@return uog�lniony deskryptor urz�dzenia lub NULL je�li otwarcie si� nie powiod�o.
		**/
		SoundDevice openDevice(SoundDeviceType type, int sample_rate, int channels = 1);
		/**
			Zamyka urz�dzenie d�wi�kowe otwarte za pomoc�
			metody openDevice().
			Niekt�re implementacje pozwalaj� na otwarcie
			wielu niezale�nie dzia�aj�cych "po��cze�"
			z urz�dzeniami. Ka�de otwarte po��czenie nale�y
			zamkn�� za pomoc� tej metody.
			Je�li w��czyli�my operacje nieblokuj�ce to
			metoda ta czeka na zako�czenie trwaj�cych operacji
			i ko�czy dzia�anie w�tk�w.
			Emituje sygna� closeDeviceImpl() w celu
			przekazania ��dania do konkrentego modu�u
			d�wi�kowego.
			@param device uog�lniony deskryptor urz�dzenia.
		**/
		void closeDevice(SoundDevice device);
		/**
			Powo�uje do �ycia w�tki zajmuj�ce si� odtwarzaniem
			i nagrywaniem pr�bek dla danego po��czenia z
			urz�dzeniem d�wi�kowym.
			Od tej chwili playSample() i recordSample()
			b�d� operacjami nieblokuj�cymi.
			@param device uog�lniony deskryptor urz�dzenia.
		**/
		void enableThreading(SoundDevice device);
		/**
			Standardowo po przekazaniu odtwarzanego sampla
			do sterownika d�wi�kowego program (w trybie blokuj�cym)
			lub w�tek odtwarzaj�cy (w trybie nieblokuj�cym) czeka
			na zako�czenie emitowania d�wi�ku przez sterownik, aby
			zagwarantowa�, �e wyj�cie z funkcji playSample() (w trybie
			blokuj�cym) lub wyemitowanie sygna�u samplePlayed() (w
			trybie nieblokuj�cym) nast�pi dopiero po fizycznym
			odegraniu d�wi�ku. Inaczej mog�o by to nast�pi� dopiero
			przy wywo�aniu metody closeDevice() co ma r�ne nieprzyjemne
			efekty uboczne.
			Czasem przy odtwarzaniu kilku ma�ych sampli jeden po drugim
			powoduje to powstanie przerw mi�dzy nimi. Aby tego unikn��
			mo�emy u�y� setFlushingEnabled(device, false) umo�liwiaj�c
			w ten spos�b p�ynne odtwarzanie kilku sampli bezpo�rednio
			po sobie.
		**/
		void setFlushingEnabled(SoundDevice device, bool enabled);
		/**
			Odtwarza pr�bk� d�wi�kow�. Standardowo jest to
			operacja blokuj�ca. Mo�e by� wywo�ana z innego
			w�tku (a nawet powinna).
			Emituje sygna� playSampleImpl() w celu
			przekazania ��dania do konkrentego modu�u
			d�wi�kowego.
			Po uprzednim wywo�aniu enableThreading() dzia�anie
			metoda jest nieblokuj�ca i przekazuje jedynie polecenie
			odtwarzania do w�tku.
			W takim wypadku nale�y uwa�a�, aby nie zwolni� pami�ci
			zajmowanej przez dane sampla zanim odtwarzanie si� nie
			zako�czy.
			@param device uog�lniony deskryptor urz�dzenia
			@param data wska�nik do danych sampla
			@param length d�ugo�� danych sampla (w bajtach)
			@return true je�li odtwarzanie zako�czy�o si� powodzeniem.
		**/
		bool playSample(SoundDevice device, const int16_t* data, int length);
		/**
			Nagrywa pr�bk� d�wi�kow�. Standardowo jest to
			operacja blokuj�ca. Mo�e by� wywo�ana z innego
			w�tku (a nawet powinna).
			Emituje sygna� recordSampleImpl() w celu
			przekazania ��dania do konkrentego modu�u
			d�wi�kowego.
			Po uprzednim wywo�aniu enableThreading() dzia�anie
			metoda jest nieblokuj�ca i przekazuje jedynie polecenie
			nagrywania do w�tku.
			W takim wypadku nale�y uwa�a�, aby nie zwolni� pami�ci
			bufora na dane sampla zanim nagrywanie si� nie
			zako�czy.
			@param device uog�lniony deskryptor urz�dzenia
			@param data wska�nik na bufor dla danych sampla
			@param length d�ugo�� sampla do nagrania (wielko�� bufora w bajtach)
			@return true je�li nagrywanie zako�czy�o si� powodzeniem.
		**/
		bool recordSample(SoundDevice device, int16_t* data, int length);

	signals:
		void playSound(const QString &sound, bool volCntrl, double vol);
		/**
			Sygna� emitowany gdy odtwarzanie sampla si�
			zako�czy�o (odnosi si� tylko do sytuacji gdy
			w��czone s� operacje nieblokuj�ce).
		**/
		void samplePlayed(SoundDevice device);
		/**
			Sygna� emitowany gdy nagrywanie sampla si�
			zako�czy�o (odnosi si� tylko do sytuacji gdy
			w��czone s� operacje nieblokuj�ce).
		**/
		void sampleRecorded(SoundDevice device);
		/**
			Pod ten sygna� powinien podpi�� si� modu�
			d�wi�kowy je�li obs�uguje funkcj� odtwarzania
			pr�bki d�wi�kowej.
			Wyemitowanie sygna�u oznacza ��danie
			otwarcia urz�dzenia d�wi�kowego do operacji
			odtwarzania i nagrywania sampli.
			@param type
			@param sample_rate sample rate - np. 8000 lub 48000
			@param channels ilo�� kana��w: 1 - mono, 2 - stereo
			@device zwr�cony uog�lniony deskryptor urz�dzenia lub NULL je�li otwarcie si� nie powiod�o.
		**/
		void openDeviceImpl(SoundDeviceType type, int sample_rate, int channels, SoundDevice& device);
		/**
			Pod ten sygna� powinien podpi�� si� modu�
			d�wi�kowy je�li obs�uguje funkcj� odtwarzania
			pr�bki d�wi�kowej.
			Wyemitowanie sygna�u oznacza ��danie
			Zamkni�cia urz�dzenia d�wi�kowego otwartegp za pomoc�
			metody openDevice().
			@param device uog�lniony deskryptor urz�dzenia.
		**/
		void closeDeviceImpl(SoundDevice device);
		/**
			Pod ten sygna� powinien podpi�� si� modu�
			d�wi�kowy je�li obs�uguje funkcj� odtwarzania
			pr�bki d�wi�kowej.
			Wyemitowanie sygna�u oznacza ��danie
			odtworzenia pr�bki d�wi�kowej.
			Modu� powinien odda� sterowanie dopiero po
			odtworzeniu pr�bki.
			Sygna� zazwyczaj b�dzie emitowany z innego
			w�tku i slot musi by� do tego przystosowany.
			@param device uog�lniony deskryptor urz�dzenia
			@param data wska�nik do danych sampla
			@param length d�ugo�� danych sampla (w bajtach)
			@param result zwr�cony rezultat operacji - true je�li odtwarzanie zako�czy�o si� powodzeniem.
		**/
		void playSampleImpl(SoundDevice device, const int16_t* data, int length, bool& result);
		/**
			Pod ten sygna� powinien podpi�� si� modu�
			d�wi�kowy je�li obs�uguje funkcj� odtwarzania
			pr�bki d�wi�kowej.
			Wyemitowanie sygna�u oznacza ��danie
			nagrania pr�bki d�wi�kowej.
			Modu� powinien odda� sterowanie dopiero po
			nagraniu pr�bki.
			Sygna� zazwyczaj b�dzie emitowany z innego
			w�tku i slot musi by� do tego przystosowany.
			@param device uog�lniony deskryptor urz�dzenia
			@param data wska�nik na bufor dla danych sampla
			@param length d�ugo�� sampla do nagrania (wielko�� bufora w bajtach)
			@param result zwr�cony rezultat operacji - true je�li nagrywanie zako�czy�o si� powodzeniem.
		**/
		void recordSampleImpl(SoundDevice device, int16_t* data, int length, bool& result);
		/**
		**/
		void setFlushingEnabledImpl(SoundDevice device, bool enabled);
};

extern SoundManager* sound_manager;

#endif
