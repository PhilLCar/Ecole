import java.util.List;
import java.util.ArrayList;
import java.util.Collections;

public class BinarySearchTree<T> {

	private Node<T> root;
	private int[] idx;
	private ArrayList<T> key;
	private int[][] H;

	public BinarySearchTree() {
		root = new Node<T>();
	}

	public int length(Node<T> s) {
		if (s.left() == null) {
			if (s.right() == null)
				return 2;
			else
				return 1 + length(s.right());
		} else {
			if (s.right() == null)
				return length(s.left()) + 1;
			else
				return length(s.left()) + length(s.right());
		}
	}

	public void fillKey(Node<T> s) {
		if (s.left() == null) {
			if (s.right() == null) {
				key.add(s.lex());
				key.add(s.rex());
			} else {
				key.add(s.lex());
				fillKey(s.right());
			}
		} else if (s.right() == null) {
			fillKey(s.left());
			key.add(s.rex());
		} else {
			fillKey(s.left());
			fillKey(s.right());
		}
	}

	@SuppressWarnings("unchecked")
	public void fillIDX() {
		key = new ArrayList<T>();
		fillKey(root);
		try {
			Collections.sort((List) key);
		} catch (Exception e) {
			return;
		}
		idx = new int[key.size()];
		fillIDX(root, 0);
	}

	public int fillIDX(Node<T> s, int n) {
		if (s.left() == null) {
			idx[key.indexOf(s.lex())] = n;
			if (s.right() == null) {
				idx[key.indexOf(s.rex())] = n + 1;
				return n + 2;
			} else {
				return fillIDX(s.right(), n + 1);
			}
		} else if (s.right() == null) {
			return (idx[key.indexOf(s.rex())] = fillIDX(s.left(), n)) + 1;
		} else {
			return fillIDX(s.right(), fillIDX(s.left(), n));
		}
	}

	public int L(Node<T> s) {
		if (s.left() == null)
			return idx[key.indexOf(s.lex())];
		return L(s.left());
	}

	public int R(Node<T> s) {
		if (s.right() == null)
			return idx[key.indexOf(s.rex())];
		return R(s.right());
	}

	public int N(Node<T> s) {
		return R(s) - L(s) + 1;
	}

	public void fillH() {
		fillIDX();
		H = new int[key.size()][2];
		fillH(root, true);
	}

	public int[] fillH(Node<T> s, boolean first) {
		if (s.left() == null) {
			int l = idx[key.indexOf(s.lex())];
			if (s.right() == null) {
				int r = idx[key.indexOf(s.rex())];

				if (first)
					H[l] = new int[] { l, r };
				else
					H[r] = new int[] { l, r };

				return new int[] { l, r, 2 };
			}
			int[] r = fillH(s.right(), true);

			if (first)
				H[l] = new int[] { l, r[1] };
			else
				H[r[1]] = new int[] { l, r[1] };

			return new int[] { l, r[1], r[2] + 1 };
		} else if (s.right() == null) {
			int r = idx[key.indexOf(s.rex())];
			int[] l = fillH(s.left(), false);

			if (first)
				H[l[0]] = new int[] { l[0], r };
			else
				H[r] = new int[] { l[0], r };

			return new int[] { l[0], r, l[2] + 1 };
		} else {
			int[] l = fillH(s.left(), false);
			int[] r = fillH(s.right(), true);

			if (first)
				H[l[0]] = new int[] { l[0], r[1] };
			else
				H[r[1]] = new int[] { l[0], r[1] };

			return new int[] { l[0], r[1], l[2] + r[2] };
		}
	}

	private boolean inH(int l, int r) {
		for (int i = 0; i < H.length; i++) {
			if (H[i][0] == l && H[i][1] == r)
				return true;
		}
		return false;
	}

	public boolean contains(Node<T> s) {
		return contains(s, true)[3] == 1;
	}

	private int[] contains(Node<T> other, boolean first) {
		int max, min;
		if (other.left() == null) {
			int l = idx[key.indexOf(other.lex())];
			if (other.right() == null) {
				int r = idx[key.indexOf(other.rex())];
				if (r < l) {
					if (l - r == 1 && inH(r, l))
						return new int[] { r, l, 2, 1 };
					else return new int[] { r, l, 2, 0 };
				}
				else {
					if (r - l == 1 && inH(l, r))
						return new int[] { l, r, 2, 1 };
					else return new int[] { l, r, 2, 0 };
				}
			}
			int[] r = contains(other.right(), true);
			min = Math.min(r[1], r[0]);
			max = Math.max(r[1], r[0]);

			if (min < l) {
				if (l - min == r[2] && inH(min, l))
					return new int[] { min, l, r[2] + 1, r[3] };
				else return new int[] { min, l, r[2] + 1, 0 };
			} else {
				if (max - l == r[2] && inH(l, r[1]))
					return new int[] { l, max, r[2] + 1, r[3] };
				else return new int[] { l, max, r[2] + 1, 0 };
			}
		} else if (other.right() == null) {
			int r = idx[key.indexOf(other.rex())];
			int[] l = contains(other.left(), false);
			min = Math.min(l[1], l[0]);
			max = Math.max(l[1], l[0]);

			if (r < max) {
				if (max - r == l[2] && inH(r, max))
					return new int[] { r, max, l[2] + 1, l[3] };
				else return new int[] { r, max, l[2] + 1, 0 };
			} else {
				if (r - min == l[2] && inH(min, r))
					return new int[] { min, r, l[2] + 1, l[3] };
				else return new int[] { min, r, l[2] + 1, 0 };
			}
		} else {
			int[] l = contains(other.left(), false);
			int[] r = contains(other.right(), true);
			max = Math.max(Math.max(Math.max(l[0], l[1]), r[0]), r[1]);
			min = Math.min(Math.min(Math.min(l[0], l[1]), r[0]), r[1]);

			if (max - min == l[2] + r[2] && inH(min, max))
				return new int[] { min, max, l[2] + r[2], l[3] & r[3] };
			else return new int[] { min, max, l[2] + r[2], 0 };
		}
	}

	public Node<T> root() {
		return root;
	}

	public int[] idx() {
		return idx;
	}

	public int[][] H() {
		return H;
	}

	public ArrayList<T> key() {
		return key;
	}

	@Override
	@SuppressWarnings("unchecked")
	public boolean equals(Object object) {
		if (!(object instanceof BinarySearchTree))
			return false;
		if (object == this)
			return true;
		return contains(((BinarySearchTree<T>) object).root(), true)[3] == 1;
	}

	@Override
	public String toString() {
		fillH();
		int max = key.size() * 2 - 1;
		int width;
		char[][] table = new char[max][key.size() * 4 - 3];
		String ret = "";

		for (int i = 0; i < table.length; i++)
			for (int j = 0; j < table[i].length; j++)
				table[i][j] = ' ';
		for (int i = 0; i < H.length; i++) {
			width = (H[i][1] - H[i][0]) * 2;
			if (width == 0)
				continue;
			table[max - width - 1][width + H[i][0] * 4] = (char) (i + 0x30);
		}
		for (int i = 0; i < idx.length; i++) {
			table[max - 1][idx[i] * 4] = key.get(i).toString().charAt(0);
		}
		for (int i = 0; i < table.length; i++) {
			for (int j = 0; j < table[i].length; j++) {
				if (i < table.length - 1) {
					switch (table[i][j]) {
					case ' ':
						break;
					case '/':
						if (table[i + 1][j - 1] == ' ')
							table[i + 1][j - 1] = '/';
						break;
					case '\\':
						if (table[i + 1][j + 1] == ' ')
							table[i + 1][j + 1] = '\\';
						break;
					default:
						table[i + 1][j - 1] = '/';
						table[i + 1][j + 1] = '\\';
						break;
					}
				}
				ret += table[i][j];
			}
			ret += '\n';
		}
		return ret;
	}
}
