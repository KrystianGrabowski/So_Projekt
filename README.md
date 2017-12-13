Wstępna specyfikacja
W swoim projekcie chciałbym zaimplementować część biblioteki do obsługi wątków z semaforami. Moja implementacja będzie zawierać podstawowe funkcje umożliwiające
obsługę wielu wątków. W jej skład będą wchodzić:

> thread_create - funkcja tworząca nowy wątek
> thread_join - funkcja czekająca na zakończenie wykonywania wątku
> thread_wait - funkcja wstrzymująca wykonywanie wątku
> thread_signal - funkcja uruchamiająca na nowo zatrzymany wątek
> funkcje implementujące jakiś rodzaj semafora

Uwzględniam możliwość dodania innych implementacji oraz usunięcia/zamiany tych, które przekroczą moje możliwości.
