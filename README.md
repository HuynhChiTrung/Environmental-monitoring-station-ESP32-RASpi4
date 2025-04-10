🌡️ Hệ Thống Giám Sát Môi Trường và Điều Khiển Từ Xa Dựa Trên ESP32 & Raspberry Pi 4

## 📘 Mô tả dự án

Dự án xây dựng hệ thống giám sát nhiệt độ và độ ẩm theo thời gian thực, sử dụng **ESP32** và **Raspberry Pi 4** để thu thập, xử lý và đồng bộ dữ liệu lên **Firebase Realtime Database**. Ngoài việc hiển thị dữ liệu trên LCD, hệ thống còn cho phép người dùng điều khiển thiết bị (quạt/đèn) từ xa thông qua ứng dụng Android.

---

![image](https://github.com/user-attachments/assets/e720abf0-b3a2-4ea6-a73a-8bbf2fd826bd)

## 🧩 Kiến trúc hệ thống

### 🔷 ESP32 – Node thu thập dữ liệu
- Kết nối cảm biến **DHT11** (giao tiếp digital) để đo nhiệt độ và độ ẩm.
- Hiển thị dữ liệu đo được trên **LCD 16x2 giao tiếp I2C**.
- Gửi dữ liệu qua **UART (Serial)** đến Raspberry Pi 4.
- Gửi dữ liệu đồng thời lên Webserver (giao thức HTTP).

### 🔷 Raspberry Pi 4 – Gateway và xử lý dữ liệu
- Nhận và phân tích dữ liệu Serial từ ESP32 để tách giá trị nhiệt độ và độ ẩm.
- Gửi dữ liệu lên **Firebase Realtime Database** bằng **Firebase SDK (Python)**.
- Đọc dữ liệu điều khiển từ Firebase (ON/OFF) để bật/tắt **quạt (LED)** thông qua GPIO.
- Tự động xử lý điều kiện môi trường:
  - Nếu nhiệt độ > 30°C → bật **LED đỏ**.
  - Nếu độ ẩm > 70% → bật **LED xanh**.

### 🔷 Ứng dụng Android
- Xây dựng bằng **MIT App Inventor**.
- Lấy dữ liệu từ Firebase và hiển thị nhiệt độ, độ ẩm theo thời gian thực.
- Cho phép người dùng điều khiển quạt từ xa bằng cách gửi tín hiệu ON/OFF lên Firebase.

---

## 🔧 Công nghệ & giao thức sử dụng

| Thành phần         | Mô tả kỹ thuật                                      |
|--------------------|------------------------------------------------------|
| Vi điều khiển      | ESP32 (Arduino Core)                                 |
| Gateway xử lý      | Raspberry Pi 4 (Python + Firebase SDK)              |
| Cảm biến           | DHT11 – đo nhiệt độ và độ ẩm                         |
| Màn hình hiển thị  | LCD 16x2 kết nối I2C                                 |
| Giao tiếp          | UART (Serial), I2C, HTTP, Firebase Realtime DB       |
| Điều khiển đầu ra  | GPIO + LED/Relay để mô phỏng quạt                    |
| Ứng dụng Android   | MIT App Inventor – giao tiếp Firebase                |

---

## 🚀 Tính năng nổi bật

- Đo và hiển thị dữ liệu cảm biến theo thời gian thực.
- Giao tiếp ổn định ESP32 ↔ Raspberry Pi thông qua UART.
- Đồng bộ dữ liệu cảm biến lên **Firebase** mỗi giây.
- Điều khiển thiết bị từ xa qua **Firebase + Android App**.
- Tự động cảnh báo và phản ứng dựa trên điều kiện môi trường.

---

## 📦 Ứng dụng thực tế

- Hệ thống giám sát nhà thông minh.
- Mô hình điều khiển môi trường trong nông nghiệp, nhà kính.
- Nền tảng mở rộng tích hợp AI/voice assistant hoặc hệ thống dự đoán.

---

> ✨ Dự án là sự kết hợp giữa lập trình nhúng, cloud và điều khiển từ xa, phù hợp triển khai trong các bài toán IoT thực tế hiện nay.
