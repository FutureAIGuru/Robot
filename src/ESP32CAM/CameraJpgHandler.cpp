#include <esp_camera.h>
#include <esp_http_server.h>
#include "esp_log.h"

#define TAG "ARDUINO"

esp_err_t jpg_httpd_handler(httpd_req_t* req)
{
    camera_fb_t* fb = NULL;
    esp_err_t res = ESP_OK;
    size_t fb_len = 0;
    int64_t fr_start = esp_timer_get_time();

    fb = esp_camera_fb_get();
    if (!fb)
    {
        ESP_LOGE(TAG, "Camera capture failed");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    res = httpd_resp_set_type(req, "image/jpeg");

    if (res == ESP_OK)
    {
        fb_len = fb->len;
        res = httpd_resp_send(req, (const char*)fb->buf, fb->len);
    }

    esp_camera_fb_return(fb);
    int64_t fr_end = esp_timer_get_time();
    ESP_LOGI(TAG, "JPG: %uKB %ums", (uint32_t)(fb_len / 1024), (uint32_t)((fr_end - fr_start) / 1000));
    return res;
}