# 🦅 The Invincible Southern Soaring Eagle

A modern, cross-platform Chinese Chess (Xiangqi) game with sleek UI, player timers, and support for future engine integration. Inspired by GUI SharkChess: https://www.sharkchess.com/.

![Screenshot](https://www.birdlife.org/wp-content/uploads/2021/06/Eagle-in-flight-Richard-Lee-Unsplash-1-edited-scaled.jpg)

---

## ✨ Features
- 🤖 Language: C++
- 🧠 Real-time player timers with visual feedback
- 🎨 Intuitive UI with styled labels and custom board
- ♟️ Traditional Chinese pieces with animated moves (planned)
- 🔌 Modular design ready for AI engine (PGN/XQF/CHE/MXQ support)
- 🧩 Easily extensible architecture for plugins and custom rules

## 📁 Project Structure

```
src/
├── main.cpp                 # Application entry point
├── chess/                   # Chess board, pieces, move generation
│   ├── ChessBoardWidget.*   # Main board widget with rendering
│   ├── Piece.*              # Piece definitions
│   ├── PieceLoader.*        # Piece image loading
│   ├── MoveGenerator.*      # Move generation logic
│   ├── BackgroundWidget.*   # Background rendering
│   ├── clockwidget.*        # Game clock widget
│   ├── CustomToolBar.*      # Custom toolbars
│   └── CustomToolBar_2.*    # Secondary toolbar
├── menus/                   # Menu system
│   ├── menu_file.*          # File operations (New, Open, Save)
│   ├── menu_position.*      # Position operations (Edit, Copy, Paste)
│   ├── menu_move.*          # Move navigation (First, Last, Step)
│   ├── menu_engine.*        # Engine settings and control
│   ├── menu_book.*          # Opening book management
│   ├── menu_connection.*    # Network connection settings
│   ├── menu_settings.*      # Game settings (sound, clock, themes)
│   ├── menu_view.*          # View options (toolbars, windows)
│   ├── menu_help.*          # Help and about
│   ├── menu_language.*      # Language selection
│   └── menu_utils.*         # Menu utilities
├── ui/                      # Main UI components
│   ├── mainwindow.*         # Main application window
│   └── mainwindow.ui        # Qt Designer UI file
├── game/                    # Game logic
│   ├── game_state.*         # Game state management
│   ├── move_data.*          # Move data structures
│   └── player.h             # Player definitions
└── dialogs/                 # Dialog windows
    └── engine_settings_dialog.*  # Engine configuration dialog
```

---

## ⚙️ Build Instructions

```bash
git clone https://github.com/minh-9999/the-invincible-southern-soaring-eagle.git
cd the-invincible-southern-soaring-eagle
mkdir build && cd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release
ninja
./chess
```

---

## 📦 Dependencies

- Qt 5.15+ or Qt 6.x
- CMake 3.16+
- Ninja (or make)
- Optional: fmtlib, nlohmann/json, ... for extended features

---

## 🚀 Roadmap

- TUI clock widget
- Board display with traditional layout
- Load/save game formats (PGN, XQF, CHE, MXQ)
- Engine integration (SharkChess, UCCI adapter)
- Multiplayer mode
- Web UI backend (optional)

---

## 🛠️ Contribution

Want to join? Contributions are welcome! Just fork, create a branch, and PR:

```
git checkout -b feature-yourFeature
```

Issues, ideas, and optimization tricks are welcome in the: https://github.com/minh-9999/the-invincible-southern-soaring-eagle/issues 

---

## 📝 License

This project is licensed under the **GNU General Public License 3.0 (GPLv3)**.

You are free to use, modify, and distribute this software, but any distributed modifications or derivative works **must also be licensed under GPLv3**. This ensures that the software and any improvements remain free and open.

For more details, see the [LICENSE](./LICENSE) file or visit the [GNU GPLv3 official page](https://www.gnu.org/licenses/gpl-3.0.en.html).


---

## 🧧 Author
  
The Invincible Southern Soaring Eagle is developed by [Minh] - a code enthusiast, chess enthusiast, and always aiming for smoothness in every line of code like every move on the chessboard.
 
## 📞 Contact

For any questions, suggestions, or collaboration opportunities, feel free to reach out:

- **Author:** Minh
- **Email:** ngo.chu.le.vuong@gmail.com
- **WeChat ID:** ChienThan88

![WeChat QR](https://i.ibb.co/qYhSfyHy/my-qr.jpg)