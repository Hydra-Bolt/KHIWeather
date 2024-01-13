#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

void sendEmailNotification(char *subject, char *body, char *recipientEmail, char *attachmentPath) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing CURL\n");
        return;
    }

    // Set the email server details
    curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

    // Set the username and password for SMTP authentication
    curl_easy_setopt(curl, CURLOPT_USERNAME, "khiweather66@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "xwhf xskw whrr bthr");

    // Set the authentication method
    curl_easy_setopt(curl, CURLOPT_LOGIN_OPTIONS, "AUTH=PLAIN");

    // Create the mime structure
    struct curl_mime *mime = curl_mime_init(curl);

    // Set the email headers using MIME structure
    struct curl_mimepart *headers_part = curl_mime_addpart(mime);
    curl_mime_data(headers_part, "From: khiweather66@gmail.com\n", CURL_ZERO_TERMINATED);
    curl_mime_data(headers_part, recipientEmail, CURL_ZERO_TERMINATED);
    char subjectHeader[100];
    snprintf(subjectHeader, sizeof(subjectHeader), "Subject: %s \r\n\r\n", subject);
    curl_mime_data(headers_part, subjectHeader, CURL_ZERO_TERMINATED);

    // Attach the message to the email
    struct curl_mimepart *part = curl_mime_addpart(mime);
    curl_mime_data(part, body, CURL_ZERO_TERMINATED); // Set the body of the email

    // Attach the PDF file as an attachment in binary mode
    part = curl_mime_addpart(mime);
    curl_mime_type(part, "application/pdf");

    FILE *pdfFile = fopen(attachmentPath, "rb"); // Open in binary mode
    if (!pdfFile) {
        fprintf(stderr, "Error opening PDF file: %s\n", attachmentPath);
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        return;
    }

    curl_mime_encoder(part, "base64"); // Use base64 encoding for binary data
    curl_mime_filedata(part, attachmentPath);

    fclose(pdfFile);

    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    // Set the recipients
    struct curl_slist *recipients = NULL;
    recipients = curl_slist_append(recipients, recipientEmail);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(curl, CURLOPT_TRANSFER_ENCODING, 1L);

    // Perform the email sending
    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK)
        fprintf(stderr, "Couldn't send Email, Due to: %s\n", curl_easy_strerror(res));

    // Clean up
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
    curl_mime_free(mime);
}
