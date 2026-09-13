# fee_payment.py
import smtplib
import ssl
import random
import sys
from email.message import EmailMessage
import qrcode
import base64
from io import BytesIO

# -------------------------
# CONFIGURE YOUR GMAIL HERE
# -------------------------
SENDER_EMAIL = "bhumisrivastav3@gmail.com"
APP_PASSWORD = "zqrkhaqsasgwpiom"  # the app password you provided

# -------------------------
# Helpers
# -------------------------
def generate_otp():
    return str(random.randint(100000, 999999))

def generate_qr_image_bytes(name, roll, amount):
    data = f"Student: {name}\nRoll No: {roll}\nFee Amount: ₹{amount}"
    qr = qrcode.QRCode(version=1, box_size=6, border=2)
    qr.add_data(data)
    qr.make(fit=True)
    img = qr.make_image(fill_color="black", back_color="white")
    bio = BytesIO()
    img.save(bio, format='PNG')
    bio.seek(0)
    return bio.read()

# -------------------------
# Send OTP via Gmail
# -------------------------
def send_otp_email(receiver_email, name, roll, amount):
    otp = generate_otp()

    # Build message
    msg = EmailMessage()
    msg['Subject'] = "Your Fee Payment OTP & QR"
    msg['From'] = SENDER_EMAIL
    msg['To'] = receiver_email
    msg.set_content(f"Hello {name},\n\nYour OTP for fee payment (Roll: {roll}) is: {otp}\n\nAmount: ₹{amount}\n\nDo not share this OTP.\n")

    # Attach QR image (optional)
    try:
        qr_bytes = generate_qr_image_bytes(name, roll, amount)
        msg.add_attachment(qr_bytes, maintype='image', subtype='png', filename=f"{roll}_fee_qr.png")
    except Exception:
        # if qrcode lib not available or attach fails, continue without QR
        pass

    try:
        # Send using SMTP SSL
        context = ssl.create_default_context()
        with smtplib.SMTP_SSL('smtp.gmail.com', 465, context=context) as server:
            server.login(SENDER_EMAIL, APP_PASSWORD)
            server.send_message(msg)

        # Print OTP to stdout (C++ reads this)
        print(otp)
    except Exception as e:
        print("ERROR_SENDING_OTP")
        print(str(e))

# -------------------------
# Main
# -------------------------
if __name__ == "__main__":
    if len(sys.argv) < 5:
        print("ERROR: Missing arguments")
        sys.exit(1)

    receiver = sys.argv[1]
    name = sys.argv[2]
    roll = sys.argv[3]
    amount = sys.argv[4]

    send_otp_email(receiver, name, roll, amount)


