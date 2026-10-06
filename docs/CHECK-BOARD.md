# Checklist Power Board — M1 s/d M4

Centang dengan ganti `[ ]` jadi `[x]` (atau klik di preview). Tulis angka di `hasil: ____`.
Multimeter analog Pioline YX1000. Lubang mengikuti rakitanmu: spine + di **kolom F**, baterai **F18 = +**, **G18 = −**.
Kalau solderan beda 1 lubang, ukur di **titik dengan fungsi yang sama**.

> **Jangan ukur ARUS (DC mA).** Motor menarik 0,5–1 A lebih, sekring multimeter bisa putus. Tegangan sudah cukup.
> **DCV:** hitam = −, merah = +. **OHM:** kebalikannya, hitam = + (baterai internal). Kalibrasi nol tiap ganti skala OHM.

---

## Tahap 0 — Firmware & app (tanpa multimeter)

- [ ] Cabut-colok USB, motor kedut bergantian ±1 detik setelah boot: M1 ☐ M2 ☐ M3 ☐ M4 ☐ (tulis yang kedut: ____)
- [ ] iPhone tersambung ke WiFi drone
- [ ] App status **connected**
- [ ] Thrust dinaikkan → minimal 1 motor berputar

> Semua kedut tapi thrust tidak memutar motor → **masalah di app/koneksi, bukan board.** Minta Claude baca log `thrust=`.
> Ada motor yang tidak kedut → cek bagian motor itu saja di Tahap 2.

## Tahap 1 — Baterai & GND (baterai terpasang, USB terpasang, J1 LEPAS), DCV 10

- [ ] Hitam G18, merah F18 = 3,7–4,2 V → hasil: ____
- [ ] Hitam GND S2 Mini, merah G18 = **0 V** (GND bersama) → hasil: ____
- [ ] Hitam GND S2 Mini, merah 3V3 S2 Mini = ±3,3 V → hasil: ____
- [ ] Hitam kolom A (A06), merah kolom K (K06) = **0 V** (J~ OK) → hasil: ____

> GND S2 ke G18 ≠ 0 → GND S2 atau B~ putus, **tidak ada motor yang bisa jalan.**
> Kolom A ke K ≠ 0 → J~ putus, M1 & M2 tidak dapat GND.

---

## Tahap 2 — Per motor

### M1 (depan-kanan, kolom A–B atas, gate dari GPIO5)

**Diam (thrust 0)**: DCV 10, probe hitam di GND kolom A
- [ ] Pin + `B01` = 3,7–4,2 V → hasil: ____
- [ ] Drain `A01` ≈ tegangan baterai → hasil: ____
- [ ] Gate `B04` = 0 V → hasil: ____
- [ ] Source `A04` = 0 V → hasil: ____

**Thrust ±50% (tanpa propeller)**
- [ ] Gate `B06` naik 0,5–3 V → hasil: ____
- [ ] Drain `A01` turun jauh di bawah tegangan baterai → hasil: ____
- [ ] Motor berputar
- [ ] AO3400 & SS14 tidak panas

**Tanpa daya, OHM** (hanya kalau ada yang gagal di atas)
- [ ] Gate `B04` ↔ Source `A04` (×1k) ±10k–22k, bukan 0/∞ → hasil: ____
- [ ] Hitam Drain → merah Source: ∞ (diam)
- [ ] Hitam Source → merah Drain: jarum gerak
- [ ] Hitam Drain → merah pin +: jarum gerak (SS14)
- [ ] Hitam pin + → merah Drain: ∞ (diam)

### M2 (belakang-kanan, kolom A–B bawah, gate dari GPIO6)

**Diam (thrust 0)**: DCV 10, probe hitam di GND kolom A
- [ ] Pin + `B18` = 3,7–4,2 V → hasil: ____
- [ ] Drain `A18` ≈ tegangan baterai → hasil: ____
- [ ] Gate `B15` = 0 V → hasil: ____
- [ ] Source `A15` = 0 V → hasil: ____

**Thrust ±50% (tanpa propeller)**
- [ ] Gate `B13` naik 0,5–3 V → hasil: ____
- [ ] Drain `A18` turun jauh di bawah tegangan baterai → hasil: ____
- [ ] Motor berputar
- [ ] AO3400 & SS14 tidak panas

**Tanpa daya, OHM** (hanya kalau ada yang gagal di atas)
- [ ] Gate `B15` ↔ Source `A15` (×1k) ±10k–22k, bukan 0/∞ → hasil: ____
- [ ] Hitam Drain → merah Source: ∞ (diam)
- [ ] Hitam Source → merah Drain: jarum gerak
- [ ] Hitam Drain → merah pin +: jarum gerak (SS14)
- [ ] Hitam pin + → merah Drain: ∞ (diam)

### M3 (belakang-kiri, kolom J–K bawah, gate dari GPIO3)

**Diam (thrust 0)**: DCV 10, probe hitam di GND kolom K
- [ ] Pin + `J18` = 3,7–4,2 V → hasil: ____
- [ ] Drain `K18` ≈ tegangan baterai → hasil: ____
- [ ] Gate `J15` = 0 V → hasil: ____
- [ ] Source `K15` = 0 V → hasil: ____

**Thrust ±50% (tanpa propeller)**
- [ ] Gate `J13` naik 0,5–3 V → hasil: ____
- [ ] Drain `K18` turun jauh di bawah tegangan baterai → hasil: ____
- [ ] Motor berputar
- [ ] AO3400 & SS14 tidak panas

**Tanpa daya, OHM** (hanya kalau ada yang gagal di atas)
- [ ] Gate `J15` ↔ Source `K15` (×1k) ±10k–22k, bukan 0/∞ → hasil: ____
- [ ] Hitam Drain → merah Source: ∞ (diam)
- [ ] Hitam Source → merah Drain: jarum gerak
- [ ] Hitam Drain → merah pin +: jarum gerak (SS14)
- [ ] Hitam pin + → merah Drain: ∞ (diam)

### M4 (depan-kiri, kolom J–K atas, gate dari GPIO4)

**Diam (thrust 0)**: DCV 10, probe hitam di GND kolom K
- [ ] Pin + `J01` = 3,7–4,2 V → hasil: ____
- [ ] Drain `K01` ≈ tegangan baterai → hasil: ____
- [ ] Gate `J04` = 0 V → hasil: ____
- [ ] Source `K04` = 0 V → hasil: ____

**Thrust ±50% (tanpa propeller)**
- [ ] Gate `J06` naik 0,5–3 V → hasil: ____
- [ ] Drain `K01` turun jauh di bawah tegangan baterai → hasil: ____
- [ ] Motor berputar
- [ ] AO3400 & SS14 tidak panas

**Tanpa daya, OHM** (hanya kalau ada yang gagal di atas)
- [ ] Gate `J04` ↔ Source `K04` (×1k) ±10k–22k, bukan 0/∞ → hasil: ____
- [ ] Hitam Drain → merah Source: ∞ (diam)
- [ ] Hitam Source → merah Drain: jarum gerak
- [ ] Hitam Drain → merah pin +: jarum gerak (SS14)
- [ ] Hitam pin + → merah Drain: ∞ (diam)

---

## Cara baca hasil

| Gejala | Biang |
|---|---|
| Pin + = 0 V | rail + tidak sampai ke motor itu |
| Drain = 0 V saat diam, motor tidak berputar | motor/konektor putus |
| Drain = 0 V saat diam, motor berputar terus | AO3400 short / Drain–Source tersambung timah |
| Gate > 0,5 V saat diam | resistor 10k tidak tersambung |
| Source > 0 V | Source tidak tersambung ke GND |
| Thrust naik, Gate tetap 0 V | kabel GPIO putus/salah pin, atau app tidak connected |
| Gate naik, Drain tidak turun | kaki AO3400 tidak tersolder / salah kaki / MOSFET rusak |
| Drain turun, motor diam | konektor atau motor rusak, tukar dengan motor yang sudah terbukti jalan |
| AO3400/SS14 panas | **cabut baterai**, kemungkinan SS14 terbalik |
| OHM Drain↔Source 0 di dua arah | AO3400 jebol / bridge timah |
| OHM pin+↔Drain 0 di dua arah | SS14 short / Drain bridge ke rail + |

## Ringkasan

- [ ] M1 OK — biang kalau gagal: ____
- [ ] M2 OK — biang kalau gagal: ____
- [ ] M3 OK — biang kalau gagal: ____
- [ ] M4 OK — biang kalau gagal: ____
