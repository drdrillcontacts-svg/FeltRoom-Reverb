# FeltRoom Reverb — GitHub Actions build

این پروژه یک پلاگین افکت ریورب VST3 مبتنی بر JUCE است، نه ساز پیانو.

## بدون نصب Visual Studio
1. در GitHub یک Repository جدید بسازید.
2. فایل‌های داخل این ZIP را در ریشه Repository آپلود کنید (پوشه‌های `Source` و `Assets` و `.github` باید در ریشه باشند).
3. تغییرات را Commit کنید.
4. وارد Actions > Build FeltRoom VST3 (Windows) > Run workflow شوید.
5. بعد از سبز شدن Build، در بخش Artifacts فایل `FeltRoom-Windows-VST3` را دانلود کنید.
6. ZIP دانلودی را باز کنید و پوشه `FeltRoom.vst3` را در `C:\Program Files\Common Files\VST3` کپی کنید؛ سپس Studio One را دوباره اسکن کنید.

**مهم:** این پروژه هنوز در GitHub Actions تست نشده و ممکن است نیاز به اصلاح داشته باشد. موتور ریورب ابتدایی است و پارامترهای PREDELAY، LOWCUT و HIGHCUT در DSP فعلی اعمال نمی‌شوند. برای استفاده حرفه‌ای باید تکمیل شوند.
