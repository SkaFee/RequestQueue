class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server)
        : num_of_null_query_(0)
        , search_server_(search_server)
    {

    }
    template <typename DocumentPredicate>
    vector<Document> AddFindRequest(const string& raw_query, DocumentPredicate document_predicate) {
        vector<Document> request = search_server_.FindTopDocuments(raw_query, document_predicate);
        QueryResult query_res = { request };
        AddRequest(query_res);
        return request;
    }

    vector<Document> AddFindRequest(const string& raw_query, DocumentStatus status) {
        vector<Document> request = search_server_.FindTopDocuments(raw_query, status);
        QueryResult query_res = { request };
        AddRequest(query_res);
        return request;
    }

    vector<Document> AddFindRequest(const string& raw_query) {
        vector<Document> request = search_server_.FindTopDocuments(raw_query);
        QueryResult query_res = { request };
        AddRequest(query_res);
        return request;
    }

    int GetNoResultRequests() const {
        return num_of_null_query_;
    }
private:
    struct QueryResult {
        vector<Document> result;
    };
    deque<QueryResult> requests_;
    const static int sec_in_day_ = 1440;
    int num_of_null_query_;
    const SearchServer& search_server_;

    void AddRequest(const QueryResult& request) {
        if (requests_.size() == sec_in_day_) {
            if (requests_.front().result.empty()) {
                --num_of_null_query_;
            }
            requests_.pop_front();
        }
        requests_.push_back(request);
        if (request.result.empty()) {
            ++num_of_null_query_;
        }
    }
};