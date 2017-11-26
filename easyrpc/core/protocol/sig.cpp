#include "sig.h"

qt_signal<bool, const std::string&> session_status_changed;
qt_signal<const request_body&, const std::shared_ptr<tcp_session>&> complete_server_decode_data;
qt_signal<const std::shared_ptr<result>&> complete_client_decode_data;
