import smtplib
import os
from os.path import basename
import sys
import argparse
from email.mime.application import MIMEApplication
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

'''
Дополнительное задание:
В качестве дополнительного параметра задается ключевое слово.
По данному ключевому слову выполняется поиск в текстовых файлах в папке клиента,
При обнаружении слова файл прикрепляется к письму.
'''

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('mail_to', action="store", help="Email address: to")
    parser.add_argument('mail_from', action="store", help="Email address: from")
    parser.add_argument('password_from', action="store",help="Password: from")
    parser.add_argument('-k', '--keyword', action="store", required=False, help="Keyword in files")
    return parser


def find_files(msg, keyword):
    if keyword == '':
        return msg

    key_files = []
    cur_dir = os.getcwd()
    file_list = os.listdir(cur_dir)
    for filename in file_list:
        if filename.endswith(".txt"):
            with open(filename) as f:
                if keyword in f.read():
                    key_files.append(filename)

    for f in key_files:
        with open(f, "rb") as fl:
            part = MIMEApplication(fl.read(), Name=basename(f))
        part['Content-Disposition'] = 'attachment; filename="%s"' % basename(f)
        msg.attach(part)

    return msg


def send_mail(msg, msg_text, smtphost, mail_from, password_from, mail_to, keyword):
    msg['From'] = mail_from
    msg['To'] = mail_to
    msg['Subject'] = "Computer networks LW5"
    msg.attach(MIMEText(msg_text, 'plain'))

    msg = find_files(msg, keyword)

    server = smtplib.SMTP(smtphost[0], smtphost[1])
    server.starttls()
    server.login(mail_from, password_from)
    server.sendmail(msg['From'], msg['To'], msg.as_string())
    server.quit()
    print("Email sent from %s to %s" % (msg['From'], msg['To']))


def main():
    parser = get_args()
    try:
        args = parser.parse_args(sys.argv[1:])
    except:
        sys.exit("Error in sending email: arguments are not valid")
    message = MIMEMultipart()
    message_text = "Hello!"
    smtphost = ["smtp.mail.ru", 25]

    if args.keyword is not None:
        send_mail(message, message_text, smtphost, args.mail_from, args.password_from, args.mail_to, args.keyword)
    else:
        send_mail(message, message_text, smtphost, args.mail_from, args.password_from, args.mail_to, '')


if __name__ == '__main__':
    main()
