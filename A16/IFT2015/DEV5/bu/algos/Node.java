
public class Node<T> {
	private Node<T> left;
	private Node<T> right;
	private Node<T> parent = null;
	private T lvalue;
	private T rvalue;
	
	public Node(Node<T> left, Node<T> right) {
		this.left = left;
		this.right = right;
		this.lvalue = null;
		this.rvalue = null;
		left.parent = this;
		right.parent = this;
	}
	
	public Node(Node<T> left, T rvalue) {
		this.left = left;
		this.right = null;
		this.lvalue = null;
		this.rvalue = rvalue;
		left.parent = this;
	}
	
	public Node(T lvalue, Node<T> right) {
		this.left = null;
		this.right = right;
		this.lvalue = lvalue;
		this.rvalue = null;
		right.parent = this;
	}
	
	public Node(T lvalue, T rvalue) {
		this.left = null;
		this.right = null;
		this.lvalue = lvalue;
		this.rvalue = rvalue;
	}
	
	public Node() {
		this.left = null;
		this.right = null;
		this.lvalue = null;
		this.rvalue = null;
	}
	
	public Node<T> left() {
		return left;
	}
	
	public Node<T> right() {
		return right;
	}
	
	public T lex() {
		return lvalue;
	}
	
	public T rex() {
		return rvalue;
	}
	
	public void setleft(Node<T> left) {
		this.left = left;
		left.parent = this;
	}
	
	public void setright(Node<T> right) {
		this.right = right;
		right.parent = this;
	}
	
	public void setleft(T lvalue) {
		this.lvalue = lvalue;
	}
	
	public void setright(T rvalue) {
		this.rvalue = rvalue;
	}
	
	public Node<T> parent() {
		return parent;
	}
}
