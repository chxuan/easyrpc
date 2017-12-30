#include "sig.h"

qt_signal<bool, std::string> session_status_changed;
qt_signal<std::shared_ptr<request>, std::shared_ptr<response>> complete_server_decode_data;
qt_signal<std::shared_ptr<result>> complete_client_decode_data;
