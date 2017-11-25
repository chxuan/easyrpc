#include "sig.h"

qt_signal<bool, const std::string&> session_status_changed;
qt_signal<const response_body&> complete_client_decode_data;
