W swoim projekcie chciałbym zaimplementować część biblioteki do obsługi wątków z semaforami. Moja implementacja będzie zawierać podstawowe funkcje umożliwiające
obsługę wielu wątków. W jej skład będą wchodzić:

1) thread_create - funkcja tworząca nowy wątek
2) thread_join - funkcja czekająca na zakończenie wykonywania wątku
3) thread_wait - funkcja wstrzymująca wykonywanie wątku
4) thread_signal - funkcja uruchamiająca na nowo zatrzymany wątek
5) funkcje implementujące jakiś rodzaj semafora

Uwzględniam możliwość dodania innych implementacji oraz usunięcia/zamiany tych, które przekroczą moje możliwości.
