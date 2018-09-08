
class ScoreElement : public sf::Drawable, public sf::Transformable {
  sf::Font *m_pFont;
  sf::Text m_text;
  std::string m_fixStr;
  int m_value;
  float x_pos;
  float y_pos;

public:
  ScoreElement(const ScoreElement &) = delete;
  ScoreElement &operator=(const ScoreElement &) = delete;
  ScoreElement();
  void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  void updateElement();
  int getVal(void);
  void setVal(int val);
  void set_reference_to_font(sf::Font *);
  void init_ScoreElem(int x, int y, int value, std::string str);
};
