#include <stdio.h>
#include <curl/curl.h>

// The function definition
void sendEmailNotification(const char *subject, const char *body, const char *recipientEmail, const char *attachmentPath);

int main() {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Specify email details
    const char *subject = "chai chaye";
    const char *body = "famma mansoor";
    const char *recipientEmail = "amubashir200018@gmail.com";
    const char *attachmentPath = "./output.pdf";

    // Call the function
    sendEmailNotification(subject, body, recipientEmail, attachmentPath);

    // Cleanup libcurl
    curl_global_cleanup();

    return 0;
}


void sendEmailNotification(const char *subject, const char *body, const char *recipientEmail, const char *attachmentPath)
{
    CURL *curl;
    CURLcode res;

    // Initialize the curl session
    curl = curl_easy_init();

    // Check if the initialization is successful
    if (curl)
    {
        // Set the email server details
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

        // Set the username and password for SMTP authentication
        curl_easy_setopt(curl, CURLOPT_USERNAME, "khiweather66@gmail.com"); // Replace with your email
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "xwhf xskw whrr bthr");           // Replace with your password

        // Set the authentication method
        curl_easy_setopt(curl, CURLOPT_LOGIN_OPTIONS, "AUTH=PLAIN");

        // Set the email headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "From: khiweather66@gmail.com");
        headers = curl_slist_append(headers, recipientEmail);
        // Add Subject to the headers
        char subjectHeader[100];
        snprintf(subjectHeader, sizeof(subjectHeader), "Subject: %s", subject);
        headers = curl_slist_append(headers, subjectHeader);
        // Set the custom headers for the email
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Create the mime structure
        struct curl_mime *mime = curl_mime_init(curl);

        // Attach the message to the email
        struct curl_mimepart *part = curl_mime_addpart(mime);
        curl_mime_data(part, body, CURL_ZERO_TERMINATED); // Set the body of the email

        // Attach the processed_data.txt as an attachment
        part = curl_mime_addpart(mime);
        curl_mime_filedata(part, "./output.pdf");

        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

        // Set the recipients
        struct curl_slist *recipients = NULL;
        recipients = curl_slist_append(recipients, recipientEmail); // Replace with recipient email
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        // Perform the email sending
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Clean up
        curl_slist_free_all(headers);
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
        curl_mime_free(mime); // Free the mime structure
    }
}